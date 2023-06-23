#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "mcu_serial_com/Serial.h"

Serial serial("/dev/ttyACM0");

class Reinterpretor {
public:
    void operator()(const std::string &message) const {
        std::cout << "arduino reinterpreted>" << message << std::endl;
    }
};

void messageCallback(const geometry_msgs::msg::Twist::SharedPtr msg) {
  serial.write(std::to_string(msg->linear.x));
  serial.write(std::to_string(msg->angular.z));
}

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("ros2_serial");

  auto subscriber = node->create_subscription<geometry_msgs::msg::Twist>(
      "cmd_vel", 10, messageCallback);

  rclcpp::spin(node);
  rclcpp::shutdown();

  return 0;
}