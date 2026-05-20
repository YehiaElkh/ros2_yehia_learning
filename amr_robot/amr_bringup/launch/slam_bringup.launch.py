import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, Command
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue

def generate_launch_description():

    # --------------------------------------------------------
    # Chemins des packages
    # --------------------------------------------------------
    amr_description_path = get_package_share_directory('amr_description')
    amr_gazebo_path      = get_package_share_directory('amr_gazebo')
    amr_slam_path        = get_package_share_directory('amr_slam')
    gazebo_ros_path      = get_package_share_directory('gazebo_ros')

    # --------------------------------------------------------
    # Fichiers
    # --------------------------------------------------------
    urdf_file   = os.path.join(amr_description_path, 'urdf', 'robot.urdf.xacro')
    world_file  = os.path.join(amr_gazebo_path, 'worlds', 'amr_world.world')
    slam_params = os.path.join(amr_slam_path, 'config', 'slam_params.yaml')

    # --------------------------------------------------------
    # Paramètres
    # --------------------------------------------------------
    use_sim_time = LaunchConfiguration('use_sim_time', default='true')

    robot_description = ParameterValue(
        Command(['xacro ', urdf_file]),
        value_type=str
    )

    # --------------------------------------------------------
    # Gazebo Server
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

    # --------------------------------------------------------
    # Gazebo Client
    # --------------------------------------------------------
    gzclient = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(gazebo_ros_path, 'launch', 'gzclient.launch.py')
        )
    )

    # --------------------------------------------------------
    # Robot State Publisher
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
    # Spawn Robot
    # --------------------------------------------------------
    spawn_robot = Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        name='spawn_amr',
        output='screen',
        arguments=[
            '-entity', 'amr_y',
            '-topic', '/robot_description',
            '-x', '0.0', '-y', '0.0', '-z', '0.05'
        ]
    )

    # --------------------------------------------------------
    # SLAM Toolbox
    # --------------------------------------------------------
    slam_toolbox = Node(
        package='slam_toolbox',
        executable='async_slam_toolbox_node',
        name='slam_toolbox',
        output='screen',
        parameters=[
            slam_params,
            {'use_sim_time': use_sim_time}
        ]
    )

    # --------------------------------------------------------
    # RViz2
    # --------------------------------------------------------
    rviz2 = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='screen'
    )

    return LaunchDescription([
        gzserver,
        gzclient,
        robot_state_publisher,
        spawn_robot,
        slam_toolbox,
        rviz2
    ])