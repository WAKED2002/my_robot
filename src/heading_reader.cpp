#include <cmath>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"

class HeadingReader : public rclcpp::Node {
public:
    HeadingReader() : Node("heading_reader") {
        sub_ = this->create_subscription<nav_msgs::msg::Odometry>(
            "/odom", 10,
            std::bind(&HeadingReader::onOdom, this, std::placeholders::_1));
    }

private:
    void onOdom(const nav_msgs::msg::Odometry::SharedPtr msg) {
        double z = msg->pose.pose.orientation.z;
        double w = msg->pose.pose.orientation.w;
        double yaw = 2.0 * std::atan2(z, w);
        double yawDeg = yaw * 180.0 / M_PI;

        RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 500,
                             "heading = %.1f deg", yawDeg);
    }

    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr sub_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<HeadingReader>());
    rclcpp::shutdown();
    return 0;
}
