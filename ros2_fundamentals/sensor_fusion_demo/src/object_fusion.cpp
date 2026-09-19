#include <cmath>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"

struct Object{
  int class_id;
  float x;
  float y;
  float confidence;
};

class ObjectFusion : public rclcpp::Node{
    public:
    ObjectFusion()
    : Node("object_fusion"),
        camera_received_(false),
        lidar_received_(false)
    {
        camera_sub_ =
        create_subscription<std_msgs::msg::Float32MultiArray>(
            "/camera/detections",
            10,
            std::bind(
            &ObjectFusion::camera_callback,
            this,
            std::placeholders::_1));

        lidar_sub_ =
        create_subscription<std_msgs::msg::Float32MultiArray>(
            "/lidar/detections",
            10,
            std::bind(
            &ObjectFusion::lidar_callback,
            this,
            std::placeholders::_1));

        fused_pub_ =
        create_publisher<std_msgs::msg::Float32MultiArray>(
            "/fusion/objects",
            10);
    }

    private:
    Object parse_detection(const std_msgs::msg::Float32MultiArray::SharedPtr msg){
        Object object{};

        if (msg->data.size() >= 4){
        object.class_id =
            static_cast<int>(msg->data[0]);

        object.x = msg->data[1];
        object.y = msg->data[2];
        object.confidence = msg->data[3];
        }

        return object;
    }

    void camera_callback(const std_msgs::msg::Float32MultiArray::SharedPtr msg){
        camera_object_ = parse_detection(msg);
        camera_received_ = true;

        try_fusion();
    }

    void lidar_callback(const std_msgs::msg::Float32MultiArray::SharedPtr msg){
        lidar_object_ = parse_detection(msg);
        lidar_received_ = true;

        try_fusion();
    }

    void try_fusion(){
        if (!camera_received_ || !lidar_received_){
        return;
        }

        const float dx = camera_object_.x - lidar_object_.x;

        const float dy = camera_object_.y - lidar_object_.y;

        const float distance = std::sqrt(dx * dx + dy * dy);

        constexpr float association_threshold = 2.0f;

        if (distance < association_threshold){
        Object fused;

        fused.class_id = camera_object_.class_id;

        fused.x =
            (camera_object_.x +
            lidar_object_.x) / 2.0f;

        fused.y =
            (camera_object_.y +
            lidar_object_.y) / 2.0f;

        fused.confidence =
            std::max(
            camera_object_.confidence,
            lidar_object_.confidence);

        std_msgs::msg::Float32MultiArray output;

        output.data = {
            static_cast<float>(fused.class_id),
            fused.x,
            fused.y,
            fused.confidence
        };

        fused_pub_->publish(output);

        RCLCPP_INFO(
            get_logger(),
            "FUSED object: class=%d position=(%.2f, %.2f) confidence=%.2f distance=%.2f",
            fused.class_id,
            fused.x,
            fused.y,
            fused.confidence,
            distance);
        }else{
        RCLCPP_WARN(
            get_logger(),
            "Objects not associated. Distance=%.2f m",
            distance);
        }

        camera_received_ = false;
        lidar_received_ = false;
    }

    rclcpp::Subscription<std_msgs::msg::Float32MultiArray>::SharedPtr camera_sub_;

    rclcpp::Subscription<std_msgs::msg::Float32MultiArray>::SharedPtr lidar_sub_;

    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr fused_pub_;

    Object camera_object_;
    Object lidar_object_;
    bool camera_received_;
    bool lidar_received_;
};

int main(int argc, char ** argv){
  rclcpp::init(argc, argv);
  auto node = std::make_shared<ObjectFusion>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}