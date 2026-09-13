from launch import LaunchDescription
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode


def generate_launch_description():

    container = ComposableNodeContainer(
        name='intra_process_container',
        namespace='',
        package='rclcpp_components',
        executable='component_container',
        composable_node_descriptions=[
            ComposableNode(
                package='intra_process_demo',
                plugin='ProducerNode',
                name='producer_node',
                extra_arguments=[
                    {'use_intra_process_comms': True}
                ],
            ),

            ComposableNode(
                package='intra_process_demo',
                plugin='ConsumerNode',
                name='consumer_node',
                extra_arguments=[
                    {'use_intra_process_comms': True}
                ],
            ),
        ],
        output='screen',
    )

    return LaunchDescription([
        container
    ])