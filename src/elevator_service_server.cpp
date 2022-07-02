#include <inttypes.h>
#include <memory>
#include "phase2_custom_interfaces/srv/detail/elevator__struct.hpp"
#include "rclcpp/rclcpp.hpp"
#include "phase2_custom_interfaces/srv/elevator.hpp"
#include "std_msgs/msg/empty.hpp"

using phase2_custom_interfaces::srv::Elevator;

rclcpp::Node::SharedPtr g_node = nullptr;

rclcpp::WallRate g_loop_rate(2);
rclcpp::Publisher<std_msgs::msg::Empty>::SharedPtr g_up_publisher;
std::shared_ptr<std_msgs::msg::Empty> g_message;

void my_handle_service(
    const std::shared_ptr<rmw_request_id_t> request_header,
    const std::shared_ptr<Elevator::Request> request,
    const std::shared_ptr<Elevator::Response> response)
{
    (void)request_header;
    RCLCPP_INFO(g_node->get_logger(),"My_callback has been called");
}

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    g_node = rclcpp::Node::make_shared("elevator_service_server");
    auto server = g_node->create_service<Elevator>("elevator", my_handle_service);

    g_up_publisher =
      g_node->create_publisher<std_msgs::msg::Empty>("elevator_up", 10);
    g_message = std::make_shared<std_msgs::msg::Empty>();

    rclcpp::spin(g_node);
    rclcpp::shutdown();
    g_node = nullptr;
    return 0;
}