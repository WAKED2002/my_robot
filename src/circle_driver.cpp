#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
using namespace std::chrono_literals;

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("circle_driver");

    auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

    auto timer = node->create_wall_timer(1000ms, [&]() {
        geometry_msgs::msg::Twist msg;
        msg.linear.x = 5.0;
        msg.angular.z = 3.0;
        publisher->publish(msg);
    });

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}