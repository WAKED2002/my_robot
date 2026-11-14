#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
using namespace std::chrono_literals;

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("pose_reader");

    float x = 0;
    float y = 0;
    float theta = 0;

    float goal_x = 8.0;
    float goal_y = 8.0;

    auto subscriber = node->create_subscription<turtlesim::msg::Pose>(
        "/turtle1/pose", 10,
        [&](turtlesim::msg::Pose::SharedPtr msg) {
            x = msg->x;
            y = msg->y;
            theta = msg->theta;
        });

    auto timer = node->create_wall_timer(1s, [&]() {
        float dx = goal_x - x;
        float dy = goal_y - y;
        float distance = std::sqrt(dx * dx + dy * dy);
        float angle_to_goal = std::atan2(dy, dx);

        RCLCPP_INFO(node->get_logger(),
            "distance=%.2f  angle_to_goal=%.2f  theta=%.2f",
            distance, angle_to_goal, theta);
    });

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}