#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/int32.hpp"
#include "message_filters/subscriber.h"
#include "message_filters/synchronizer.h"
#include "message_filters/sync_policies/approximate_time.h"

typedef message_filters::sync_policies::ApproximateTime<std_msgs::msg::String, std_msgs::msg::Int32> SyncPolicy;
typedef message_filters::Synchronizer<SyncPolicy> Synchronizer;

class SyncSubscriber : public rclcpp::Node
{
public:
    SyncSubscriber() : Node("sync_subscriber")
    {
        sub1_ = std::make_shared<message_filters::Subscriber<std_msgs::msg::String>>(
            this, "camera_data"
        );

        sub2_ = std::make_shared<message_filters::Subscriber<std_msgs::msg::Int32>>(
            this, "lidar_data"
        );

        sync_ = std::make_shared<Synchronizer>(SyncPolicy(10), *sub1_, *sub2_);

        sync_->registerCallback(
            std::bind(&SyncSubscriber::syncCallback, this,
                std::placeholders::_1,
                std::placeholders::_2)
        );

        RCLCPP_INFO(this->get_logger(), "Sync Subscriber started !");
    }

private:
    void syncCallback(
        const std_msgs::msg::String::ConstSharedPtr msg1,
        const std_msgs::msg::Int32::ConstSharedPtr msg2)
    {
        RCLCPP_INFO(this->get_logger(),
            "SYNCED → camera: '%s' | lidar: %d",
            msg1->data.c_str(), msg2->data);
    }

    std::shared_ptr<message_filters::Subscriber<std_msgs::msg::String>> sub1_;
    std::shared_ptr<message_filters::Subscriber<std_msgs::msg::Int32>>  sub2_;
    std::shared_ptr<Synchronizer> sync_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SyncSubscriber>());
    rclcpp::shutdown();
    return 0;
}