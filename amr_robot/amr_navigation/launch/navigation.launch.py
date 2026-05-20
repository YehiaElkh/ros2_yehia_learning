import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, Command
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue

def generate_launch_description():

    # --------------------------------------------------------
    # Chemins
    # --------------------------------------------------------
    amr_description_path = get_package_share_directory('amr_description')
    amr_gazebo_path      = get_package_share_directory('amr_gazebo')
    amr_navigation_path  = get_package_share_directory('amr_navigation')
    gazebo_ros_path      = get_package_share_directory('gazebo_ros')

    urdf_file   = os.path.join(amr_description_path, 'urdf', 'robot.urdf.xacro')
    world_file  = os.path.join(amr_gazebo_path, 'worlds', 'amr_world.world')
    nav2_params = os.path.join(amr_navigation_path, 'config', 'nav2_params.yaml')
    map_file    = os.path.join(amr_navigation_path, 'maps', 'amr_map.yaml')

    # --------------------------------------------------------
    # Paramètres
    # --------------------------------------------------------
    use_sim_time = LaunchConfiguration('use_sim_time', default='true')

    robot_description = ParameterValue(
        Command(['xacro ', urdf_file]),
        value_type=str
    )

    # --------------------------------------------------------
    # ÉTAPE 1 : Gazebo (démarrer en premier)
    # --------------------------------------------------------
    gzserver = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(gazebo_ros_path, 'launch', 'gzserver.launch.py')
        ),
        launch_arguments={
            'world': world_file,
            'pause': 'false'
        }.items()
    )

    gzclient = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(gazebo_ros_path, 'launch', 'gzclient.launch.py')
        )
    )

    # --------------------------------------------------------
    # ÉTAPE 2 : Robot State Publisher
    # --------------------------------------------------------
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='screen',
        parameters=[{
            'robot_description': robot_description,
            'use_sim_time': use_sim_time
        }]
    )

    # --------------------------------------------------------
    # ÉTAPE 3 : Spawner le robot
    # --------------------------------------------------------
    spawn_robot = Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        name='spawn_amr',
        output='screen',
        arguments=[
            '-entity', 'amr_y',
            '-topic', '/robot_description',
            '-x', '0.0',
            '-y', '0.0',
            '-z', '0.05'
        ]
    )

    # --------------------------------------------------------
    # ÉTAPE 4 : Nav2 — démarrer après 5 secondes
    # --------------------------------------------------------
    # TimerAction : attendre N secondes avant de lancer
    # Donne le temps à Gazebo de démarrer et au robot d'être spawné
    nav2_bringup = TimerAction(
        period=5.0,
        # Attendre 5 secondes après le démarrage de Gazebo
        actions=[
            IncludeLaunchDescription(
                PythonLaunchDescriptionSource(
                    os.path.join(
                        get_package_share_directory('nav2_bringup'),
                        'launch',
                        'bringup_launch.py'
                        # bringup_launch.py inclut map_server + amcl + nav2
                    )
                ),
                launch_arguments={
                    'use_sim_time': 'true',
                    'params_file': nav2_params,
                    'map': map_file,
                    # Passer le fichier de carte directement
                }.items()
            )
        ]
    )

    # --------------------------------------------------------
    # ÉTAPE 5 : RViz — démarrer après 3 secondes
    # --------------------------------------------------------
    rviz2 = TimerAction(
        period=3.0,
        actions=[
            Node(
                package='rviz2',
                executable='rviz2',
                name='rviz2',
                output='screen'
            )
        ]
    )

    return LaunchDescription([
        gzserver,               # 1. Moteur physique Gazebo
        gzclient,               # 2. Interface Gazebo
        robot_state_publisher,  # 3. TF2 robot
        spawn_robot,            # 4. Spawner le robot
        nav2_bringup,           # 5. Nav2 après 5s
        rviz2,                  # 6. RViz après 3s
    ])