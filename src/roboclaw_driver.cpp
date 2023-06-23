#include <memory>
#include <iostream>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "mcu_serial_com/Serial.h"

using std::placeholders::_1;

Serial serial("/dev/ttyACM0");

class MinimalSubscriber : public rclcpp::Node
{
  public:
    MinimalSubscriber()
    : Node("cmd_vel_node")
    {
      subscription_ = this->create_subscription<geometry_msgs::msg::Twist>(
      "cmd_vel", 10, std::bind(&MinimalSubscriber::topic_callback, this, _1));
    }

  private:
    void topic_callback(const geometry_msgs::msg::Twist::SharedPtr msg) const
    {
      //RCLCPP_INFO(this->get_logger(), "Linear x: '%f'", msg->linear.x);
      //RCLCPP_INFO(this->get_logger(), "Angular z: '%f'", msg->angular.z);
      serial.write("S " + std::to_string(msg->linear.x) + " " + std::to_string(msg->angular.z));
      //serial.write(std::to_string(msg->angular.z));
    }
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalSubscriber>());
  rclcpp::shutdown();
  return 0;
}
