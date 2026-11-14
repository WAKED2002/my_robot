#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/twist.hpp"
using namespace std::chrono_literals;

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("go_to_goal");

    float x = 0;
    float y = 0;
    float theta = 0;

    float goal_x = 10.0;
    float goal_y = 9.0;

    // SENSE: listen to where the turtle is
    auto subscriber = node->create_subscription<turtlesim::msg::Pose>(
        "/turtle1/pose", 10,
        [&](turtlesim::msg::Pose::SharedPtr msg) {
            x = msg->x;
            y = msg->y;
            theta = msg->theta;
        });

    // ACT: a microphone to send speed commands
    auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

    // THINK: runs 10 times per second
    auto timer = node->create_wall_timer(100ms, [&]() {
        float dx = goal_x - x;
        float dy = goal_y - y;
        float distance = std::sqrt(dx * dx + dy * dy);
        float angle_to_goal = std::atan2(dy, dx);

        float error = angle_to_goal - theta;
        error = std::atan2(std::sin(error), std::cos(error));

        geometry_msgs::msg::Twist msg;

        if (distance < 0.1) {
            // arrived: msg stays all zeros, so the turtle stops
        } else if (std::fabs(error) > 0.3) {
            // facing the wrong way: turn on the spot
            msg.angular.z = 2.0 * error;
        } else {
            // facing the goal: drive and keep correcting
            msg.linear.x = 1.0 * distance;
            msg.angular.z = 2.0 * error;
        }

        publisher->publish(msg);
    });

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}