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
    std::shared_ptr<Elevator::Response> response)
{
    (void)request_header;
    
    RCLCPP_INFO_STREAM(
        g_node->get_logger(),
        "Elevator service incoming request - is_up: " << (request->is_up ? "true" : "false"));

    if (!request->is_up)
    {
        response->set__success(false);
    }

    const int attempt_limit = 1;
    int attempts = 0;
    while (rclcpp::ok() && (attempts < attempt_limit)) {
        g_up_publisher->publish(*g_message);
        //rclcpp::spin_some(g_elevator_service_node);
        g_loop_rate.sleep();
        attempts += 1;
    }

    response->set__success(true);
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