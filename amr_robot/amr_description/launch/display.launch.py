import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.substitutions import LaunchConfiguration, Command
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
# ParameterValue : indique explicitement le type du paramètre
# Nécessaire quand la valeur est générée par une commande (xacro)

rviz_config = os.path.join(pkg_path, 'rviz', 'amr_display.rviz')

def generate_launch_description():

    # Chemin vers le fichier URDF
    pkg_path = get_package_share_directory('amr_description')
    urdf_file = os.path.join(pkg_path, 'urdf', 'robot.urdf.xacro')

    # Convertir xacro → URDF et forcer le type string
    robot_description = ParameterValue(
        Command(['xacro ', urdf_file]),
        value_type=str
        # value_type=str : dire explicitement à ROS2 que c'est une string
        # Sans ça → ROS2 essaie de parser le XML comme du YAML → erreur
    )

    # Paramètre use_sim_time
    use_sim_time = LaunchConfiguration('use_sim_time', default='false')

    # NŒUD 1 : robot_state_publisher
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

    # NŒUD 2 : joint_state_publisher_gui
    joint_state_publisher_gui = Node(
        package='joint_state_publisher_gui',
        executable='joint_state_publisher_gui',
        name='joint_state_publisher_gui',
        output='screen'
    )

    # NŒUD 3 : rviz2
    rviz2 = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='screen',
        arguments=['-d', rviz_config]
    )

    return LaunchDescription([
        robot_state_publisher,
        joint_state_publisher_gui,
        rviz2
    ])