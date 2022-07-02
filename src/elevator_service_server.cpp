#include <inttypes.h>
#include <memory>
#include "phase2_custom_interfaces/srv/detail/elevator__struct.hpp"
#include "rclcpp/rclcpp.hpp"
#include "phase2_custom_interfaces/srv/elevator.hpp"

using phase2_custom_interfaces::srv::Elevator;
// using Empty = std_srvs::srv::Empty;
rclcpp::Node::SharedPtr g_node = nullptr;

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
    rclcpp::spin(g_node);
    rclcpp::shutdown();
    g_node = nullptr;
    return 0;
}