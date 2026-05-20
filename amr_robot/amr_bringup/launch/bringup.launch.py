import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, DeclareLaunchArgument
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch.conditions import IfCondition, UnlessCondition
from launch.substitutions import PythonExpression

def generate_launch_description():

    # --------------------------------------------------------
    # Chemin du package bringup
    # --------------------------------------------------------
    amr_bringup_path = get_package_share_directory('amr_bringup')

    # --------------------------------------------------------
    # ARGUMENT MODE — choisir le mode de lancement
    # --------------------------------------------------------
    mode_arg = DeclareLaunchArgument(
        'mode',
        default_value='navigation',
        description='Mode de lancement : slam | navigation | mission | display'
        # slam        : Gazebo + SLAM (cartographie)
        # navigation  : Gazebo + Nav2 (navigation autonome)
        # mission     : Gazebo + Nav2 + Mission Planner
        # display     : RViz uniquement (visualisation URDF)
    )

    mode = LaunchConfiguration('mode')

    # --------------------------------------------------------
    # MODE SLAM
    # --------------------------------------------------------
    slam_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(amr_bringup_path, 'launch', 'slam_bringup.launch.py')
        ),
        condition=IfCondition(PythonExpression(["'", mode, "' == 'slam'"]))
        # Lancer seulement si mode == 'slam'
    )

    # --------------------------------------------------------
    # MODE NAVIGATION
    # --------------------------------------------------------
    nav_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(amr_bringup_path, 'launch', 'nav_bringup.launch.py')
        ),
        condition=IfCondition(PythonExpression(["'", mode, "' == 'navigation'"]))
        # Lancer seulement si mode == 'navigation'
    )

    # --------------------------------------------------------
    # MODE MISSION
    # --------------------------------------------------------
    mission_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(amr_bringup_path, 'launch', 'mission_bringup.launch.py')
        ),
        condition=IfCondition(PythonExpression(["'", mode, "' == 'mission'"]))
        # Lancer seulement si mode == 'mission'
    )

    # --------------------------------------------------------
    # MODE DISPLAY
    # --------------------------------------------------------
    display_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(
                get_package_share_directory('amr_description'),
                'launch', 'display.launch.py'
            )
        ),
        condition=IfCondition(PythonExpression(["'", mode, "' == 'display'"]))
        # Lancer seulement si mode == 'display'
    )

    return LaunchDescription([
        mode_arg,
        slam_launch,
        nav_launch,
        mission_launch,
        display_launch
    ])