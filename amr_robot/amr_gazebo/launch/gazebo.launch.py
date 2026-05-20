import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, ExecuteProcess
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, Command
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue

def generate_launch_description():

    # --------------------------------------------------------
    # Chemins vers les packages
    # --------------------------------------------------------
    amr_description_path = get_package_share_directory('amr_description')
    amr_gazebo_path      = get_package_share_directory('amr_gazebo')
    gazebo_ros_path      = get_package_share_directory('gazebo_ros')
    # get_package_share_directory : trouve le chemin du package installé

    # --------------------------------------------------------
    # Fichiers
    # --------------------------------------------------------
    urdf_file  = os.path.join(amr_description_path, 'urdf', 'robot.urdf.xacro')
    world_file = os.path.join(amr_gazebo_path, 'worlds', 'amr_world.world')

    # --------------------------------------------------------
    # Paramètres
    # --------------------------------------------------------
    use_sim_time = LaunchConfiguration('use_sim_time', default='true')
    # use_sim_time=true : synchroniser tous les nœuds avec l'horloge Gazebo

    x_pose = LaunchConfiguration('x_pose', default='0.0')
    y_pose = LaunchConfiguration('y_pose', default='0.0')
    # Position initiale du robot dans le monde

    # --------------------------------------------------------
    # Robot description depuis xacro
    # --------------------------------------------------------
    robot_description = ParameterValue(
        Command(['xacro ', urdf_file]),
        value_type=str
    )

    # --------------------------------------------------------
    # NŒUD 1 : Gazebo Server (moteur physique)
    # --------------------------------------------------------
    # gzserver : processus principal de simulation
    # Charge le monde et calcule la physique
    # Tourne sans interface graphique
    gzserver = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(gazebo_ros_path, 'launch', 'gzserver.launch.py')
        ),
        launch_arguments={
            'world': world_file,
            # Charger notre monde personnalisé
            'pause': 'false'
            # false : démarrer la simulation immédiatement
        }.items()
    )

    # --------------------------------------------------------
    # NŒUD 2 : Gazebo Client (interface graphique)
    # --------------------------------------------------------
    # gzclient : interface graphique de Gazebo
    # Se connecte au gzserver
    # Peut être lancé séparément ou plus tard
    gzclient = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(gazebo_ros_path, 'launch', 'gzclient.launch.py')
        )
    )

    # --------------------------------------------------------
    # NŒUD 3 : robot_state_publisher
    # --------------------------------------------------------
    # Publie les transformations TF2 du robot
    # Lit le URDF et publie /tf pour chaque joint
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
    # NŒUD 4 : spawn_entity
    # --------------------------------------------------------
    # Spawner le robot dans Gazebo
    # Lit le URDF depuis /robot_description et place le robot dans le monde
    spawn_robot = Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        # spawn_entity.py : script Python qui place un modèle dans Gazebo
        name='spawn_amr',
        output='screen',
        arguments=[
            '-entity', 'amr_y',
            # -entity : nom du robot dans Gazebo
            '-topic', '/robot_description',
            # -topic : lire le URDF depuis ce topic
            '-x', x_pose,
            '-y', y_pose,
            '-z', '0.05',
            # Position initiale du robot
            '-R', '0', '-P', '0', '-Y', '0'
            # Orientation initiale (Roll Pitch Yaw)
        ]
    )

    return LaunchDescription([
        gzserver,               # 1. Moteur physique
        gzclient,               # 2. Interface graphique
        robot_state_publisher,  # 3. TF2 du robot
        spawn_robot,            # 4. Placer le robot dans Gazebo
    ])