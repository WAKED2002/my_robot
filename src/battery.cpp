#include "rclcpp/rclcpp.hpp" // Brings in ROS 2, like #include <iostream> brings in cout.
#include "std_msgs/msg/float32.hpp" //Brings in the message type Float32, which is just one decimal number. ROS 2 has many message types: a number, text, a speed command, a lidar scan, a camera image. We need the simplest one.
using namespace std::chrono_literals; 










int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv); // ROS 2, turn on. Always the first line.

    auto node = std::make_shared<rclcpp::Node>("battery"); // Creates a node named battery. Read it as: make a ROS 2 Node called battery, and keep it in a smart pointer named node
    float level = 100 ;
    auto publisher = node->create_publisher<std_msgs::msg::Float32>("battery_level", 10); // Read it as: “node, make a publisher that sends Float32 numbers on the topic battery_level.

    RCLCPP_INFO(node->get_logger() , "Hello ! im a ROS 2 node. "); //ROS 2’s version of cout. It prints the text, plus the time and the node’s name. We use -> because node is a pointer.
    auto timer = node->create_wall_timer(1s, [&](){      // Read it as: “node, make a timer that rings every 1 second, and each time it rings, run the code inside { }.”
        std_msgs::msg::Float32 msg; // Creates an empty message, like an empty envelope. It’s a normal object, not a pointer.
        msg.data = level; // Puts the number inside. A Float32 message has one field called data. We use . because msg is a normal object.
        publisher->publish(msg); // Sends it. We use -> because publisher is a smart pointer.
        
        
        
        RCLCPP_INFO (node->get_logger(), "Battery: %.0f%%", level);
        level = level - 5;
        if (level < 0) {
            level = 100 ;
        }

    });



    rclcpp::spin(node);
    rclcpp::shutdown(); // “ROS 2, turn off.” Always the last line. 



    return 0;
}

// turn on → create a node → print hello → turn off.

// start → create node → create timer → spin (wait...)
                                        //↓ every 1 second
                             // print level, level − 5