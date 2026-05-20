#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
//       ↑ message pour représenter une transformation

#include "tf2_ros/transform_broadcaster.h"
//       ↑ bibliothèque pour publier des transformations TF2

#include <cmath>
//       ↑ pour les fonctions mathématiques (sin, cos...)

class DynamicTF2Publisher : public rclcpp::Node
{
public:
    DynamicTF2Publisher() : Node("dynamic_tf2_publisher"), angle_(0.0)
    {
        // ----------------------------------------------------
        // DÉCLARER LES PARAMÈTRES
        // ----------------------------------------------------
        this->declare_parameter("parent_frame", std::string("map"));
        this->declare_parameter("child_frame",  std::string("base_link"));
        this->declare_parameter("x",    0.0);
        this->declare_parameter("y",    0.0);
        this->declare_parameter("z",    0.0);
        this->declare_parameter("yaw",  0.0);

        // Lire les paramètres
        parent_frame_ = this->get_parameter("parent_frame").as_string();
        child_frame_  = this->get_parameter("child_frame").as_string();
        x_   = this->get_parameter("x").as_double();
        y_   = this->get_parameter("y").as_double();
        z_   = this->get_parameter("z").as_double();
        yaw_ = this->get_parameter("yaw").as_double();

        // ----------------------------------------------------
        // CRÉER LE BROADCASTER TF2
        // ----------------------------------------------------
        tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);
        //                  ↑ publie les transformations sur /tf

        // Timer : publier la transformation toutes les 100ms
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&DynamicTF2Publisher::timerCallback, this)
        );

        RCLCPP_INFO(this->get_logger(),
            "TF2 Publisher started : %s → %s",
            parent_frame_.c_str(), child_frame_.c_str());
    }

private:
    void timerCallback()
    {
        // ----------------------------------------------------
        // CRÉER LE MESSAGE DE TRANSFORMATION
        // ----------------------------------------------------
        geometry_msgs::msg::TransformStamped transform;

        // En-tête : timestamp et frame parent
        transform.header.stamp    = this->get_clock()->now();
        //                           ↑ timestamp actuel
        transform.header.frame_id = parent_frame_;
        //                           ↑ frame de référence (parent)

        // Frame enfant
        transform.child_frame_id = child_frame_;

        // ----------------------------------------------------
        // TRANSLATION (position x, y, z)
        // ----------------------------------------------------
        transform.transform.translation.x = x_;
        transform.transform.translation.y = y_;
        transform.transform.translation.z = z_;

        // ----------------------------------------------------
        // ROTATION (quaternion)
        // TF2 utilise des quaternions pour les rotations
        // On convertit l'angle yaw en quaternion
        // ----------------------------------------------------
        // Pour une rotation autour de Z (yaw) :
        // w = cos(angle/2)
        // z = sin(angle/2)
        // x = y = 0
        transform.transform.rotation.x = 0.0;
        transform.transform.rotation.y = 0.0;
        transform.transform.rotation.z = std::sin(angle_ / 2.0);
        transform.transform.rotation.w = std::cos(angle_ / 2.0);

        // Faire tourner lentement pour visualiser
        angle_ += 0.01;
        if (angle_ > 2 * M_PI) angle_ = 0.0;
        //                 ↑ remettre à zéro après un tour complet

        // Publier la transformation
        tf_broadcaster_->sendTransform(transform);
    }

    std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
    rclcpp::TimerBase::SharedPtr timer_;

    std::string parent_frame_;
    std::string child_frame_;
    double x_, y_, z_, yaw_;
    double angle_;   // angle courant pour l'animation
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DynamicTF2Publisher>());
    rclcpp::shutdown();
    return 0;
}