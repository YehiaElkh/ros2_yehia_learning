import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

def generate_launch_description():

    pkg_path = get_package_share_directory('amr_controller')

    # Fichier de mission par défaut
    mission_file = os.path.join(pkg_path, 'config', 'mission.yaml')

    # Paramètre auto_start depuis le terminal
    auto_start = LaunchConfiguration('auto_start', default='true')

    # Nœud mission planner
    mission_planner = Node(
        package='amr_controller',
        executable='mission_planner',
        name='mission_planner',
        output='screen',
        parameters=[{
            'mission_file': mission_file,
            # Charger le fichier de mission
            'auto_start': auto_start
            # Démarrer automatiquement
        }]
    )

    return LaunchDescription([
        mission_planner
    ])
