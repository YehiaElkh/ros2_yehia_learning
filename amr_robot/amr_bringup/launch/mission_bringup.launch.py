import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

def generate_launch_description():

    # --------------------------------------------------------
    # Chemins
    # --------------------------------------------------------
    amr_bringup_path    = get_package_share_directory('amr_bringup')
    amr_controller_path = get_package_share_directory('amr_controller')

    # --------------------------------------------------------
    # Fichier de mission
    # --------------------------------------------------------
    mission_file = os.path.join(
        amr_controller_path, 'config', 'mission.yaml')

    # --------------------------------------------------------
    # Inclure nav_bringup (Gazebo + Nav2)
    # --------------------------------------------------------
    nav_bringup = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(amr_bringup_path, 'launch', 'nav_bringup.launch.py')
        )
    )

    # --------------------------------------------------------
    # Mission Planner — avec délai pour attendre Nav2
    # --------------------------------------------------------
    mission_planner = TimerAction(
        period=10.0,
        # Attendre 10 secondes que Nav2 soit complètement prêt
        actions=[
            Node(
                package='amr_controller',
                executable='mission_planner',
                name='mission_planner',
                output='screen',
                parameters=[{
                    'mission_file': mission_file,
                    'auto_start': True
                    # Démarrer automatiquement après le délai
                }]
            )
        ]
    )

    return LaunchDescription([
        nav_bringup,
        mission_planner
    ])