#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "geometry_msgs/msg/twist.hpp"
using namespace std::chrono_literals;

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("robot_go_to_goal");

    float x = 0;
    float y = 0;
    float yaw = 0;

    // the goal, in meters from where the robot started
    float goal_x = -1.0;
    float goal_y = 0.0;

    // SENSE: where am I?
    auto subscriber = node->create_subscription<nav_msgs::msg::Odometry>(
        "/odom", 10,
        [&](nav_msgs::msg::Odometry::SharedPtr msg) {
            x = msg->pose.pose.position.x;
            y = msg->pose.pose.position.y;

            float qx = msg->pose.pose.orientation.x;
            float qy = msg->pose.pose.orientation.y;
            float qz = msg->pose.pose.orientation.z;
            float qw = msg->pose.pose.orientation.w;
            yaw = std::atan2(2.0 * (qw * qz + qx * qy),
                             1.0 - 2.0 * (qy * qy + qz * qz));
        });

    // ACT: send driving commands
    auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

    // THINK: 10 times per second
    auto timer = node->create_wall_timer(100ms, [&]() {
        float dx = goal_x - x;
        float dy = goal_y - y;
        float distance = std::sqrt(dx * dx + dy * dy);
        float angle_to_goal = std::atan2(dy, dx);

        float error = angle_to_goal - yaw;
        error = std::atan2(std::sin(error), std::cos(error));

        geometry_msgs::msg::Twist msg;

        if (distance < 0.05) {
            // arrived: msg stays all zeros, so the robot stops
        } else if (std::fabs(error) > 0.3) {
            // facing the wrong way: turn on the spot
            msg.angular.z = 1.5 * error;
        } else {
            // facing the goal: drive and keep correcting
            msg.linear.x = 0.5 * distance;
            if (msg.linear.x > 0.5) {
                msg.linear.x = 0.5;      // speed limit
            }
            msg.angular.z = 1.5 * error;
        }

        publisher->publish(msg);
    });

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}