#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("color_finder");

    auto subscriber = node->create_subscription<sensor_msgs::msg::Image>(
        "/image_raw", 10,
        [&](sensor_msgs::msg::Image::SharedPtr msg) {
            int count = 0;
            long sum_x = 0;

            for (unsigned int y = 0; y < msg->height; y++) {
                for (unsigned int x = 0; x < msg->width; x++) {
                    int i = y * msg->step + x * 3;
                    int r = msg->data[i];
                    int g = msg->data[i + 1];
                    int b = msg->data[i + 2];

                    if (b > r + 50 && b > g + 50) {
                        count = count + 1;
                        sum_x = sum_x + x;
                    }
                }
            }

            if (count < 50) {
                RCLCPP_INFO(node->get_logger(), "no blue box in view");
                return;
            }

            float center_x = (float)sum_x / count;
            const char * where = "CENTER";
            if (center_x < msg->width / 3.0) {
                where = "LEFT";
            } else if (center_x > 2.0 * msg->width / 3.0) {
                where = "RIGHT";
            }

            RCLCPP_INFO(node->get_logger(),
                "blue box: %d pixels, x=%.0f -> %s", count, center_x, where);
        });

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}