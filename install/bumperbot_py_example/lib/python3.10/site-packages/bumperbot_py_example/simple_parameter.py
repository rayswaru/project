#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from rcl_interfaces.msg import SetParametersResult
from rclpy.parameter import Parameter


class SimpleParameter(Node): 
    def __init__(self):
        super().__init__("simple_parameter")

        self.declare_parameter("simple_int_param",20)
        self.declare_parameter("simple_string_param","Ray")

        self.add_on_set_parameters_callback(self.parameterCallback)


    def parameterCallback(self , params):
        result = SetParametersResult()

        for param in params:
            if param.name == "simple_int_param" and param.type_ == Parameter.Type.INTEGER:
                self.get_logger().info(
                    "Param simple_int_param chaged new value is %d" % param.value)
                result.successful = True

            if param.name == "simple_string_param" and param.type_ == Parameter.Type.STRING:
                self.get_logger().info(
                    "Param simple_string_param chaged new value is %s" % param.value)
                result.successful = True

            return result

def main(args=None):
    rclpy.init(args=args)
    node = SimpleParameter()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()