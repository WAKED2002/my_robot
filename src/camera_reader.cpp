#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("camera_reader");

    auto subscriber = node->create_subscription<sensor_msgs::msg::Image>(
        "/image_raw", 10,
        [&](sensor_msgs::msg::Image::SharedPtr msg) {
            long total = 0;
            for (auto value : msg->data) {
                total = total + value;
            }
            float average = (float)total / msg->data.size();

            RCLCPP_INFO(node->get_logger(),
                "picture %dx%d  brightness=%.1f (0=black, 255=white)",
                msg->width, msg->height, average);
        });

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}