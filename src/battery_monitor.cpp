#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("battery_monitor");

    auto subscriber = node->create_subscription<std_msgs::msg::Float32>(   // create_subscription is the opposite of create_publisher.
        "battery_level", 10,
        [&](std_msgs::msg::Float32::SharedPtr msg) {   // This is a lambda, like your timer one. The only difference is the ( ... ) after [&]. The timer one was empty () because nothing was handed to it. This one receives the message that just arrived, and calls it msg.
            if (msg->data < 30) {
                RCLCPP_WARN(node->get_logger(), "LOW BATTERY: %.0f%%", msg->data);
            } else {
                RCLCPP_INFO(node->get_logger(), "Battery OK: %.0f%%", msg->data);
            }
        });

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}