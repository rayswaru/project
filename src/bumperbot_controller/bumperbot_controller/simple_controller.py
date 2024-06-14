#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from std_msgs.msg import Float64MultiArray
from geometry_msgs.msg import TwistStamped
from sensor_msgs.msg import JointState
from rclpy.time import Time
from rclpy.constants import S_TO_NS
import numpy as np
import math

class SimpleController(Node):
    def __init__(self):
        super().__init__("simple_controller")

        self.declare_parameter("wheel_radius",0.033)
        self.declare_parameter("wheel_separation",0.17)
        self.left_wheel_prev_pos_ = 0.0
        self.right_wheel_prev_pos_ = 0.0
        self.prev_time_ = self.get_clock().now()
        self.x_ = 0.0
        self.y_ = 0.0
        self.theta_ = 0.0

        self.wheel_radius_ = self.get_parameter("wheel_radius"
                                                ).get_parameter_value().double_value
        self.wheel_separation_ = self.get_parameter("wheel_separation"
                                                    ).get_parameter_value().double_value

        self.get_logger().info("Using wheel_radius %f\n"%self.wheel_radius_)
        self.get_logger().info("Using wheel_separation %f\n"%self.wheel_separation_)

        self.wheel_cmd_pub_ = self.create_publisher(Float64MultiArray,
                                                    "simple_velocity_controller/command",10)
        self.vel_sub_ = self.create_subscription(TwistStamped,
                                                 "bumperbot_controller/cmd_vel",self.velCallback,10)
        self.joint_sub_ = self.create_subscription(JointState,"joint_state",self.jointCallback,10)

        self.speed_conversion_ = np.array([[self.wheel_radius_/2,self.wheel_radius_/2],
                                           [self.wheel_radius_/self.wheel_separation_,
                                            -self.wheel_radius_/self.wheel_separation_]])
        
        self.get_logger().info("The conversion matrix is %s: " %self.speed_conversion_)


    def velCallback(self,msg):

        robot_speed = np.array([[msg.twist.linear.x],
                                [msg.twist.angular.z]])
        wheel_speed = np.matmul(np.linalg.inv(self.speed_conversion_),robot_speed)
        wheel_speed_msg = Float64MultiArray()

        wheel_speed_msg.data = [wheel_speed[1,0],wheel_speed[0,0]]
        self.wheel_cmd_pub_.publish(wheel_speed_msg)

    def jointCallback(self,msg):
        dp_left = msg.position[1] - self.left_wheel_prev_pos_
        dp_right = msg.position[0] - self.right_wheel_prev_pos_
        dt = Time.from_msg(msg.header.stamp) - self.prev_time_

        self.left_wheel_prev_pos_ = msg.position[1]
        self.right_wheel_prev_pos_ = msg.position[0]
        self.prev_time_ = Time.from_msg(msg.header.stamp)

        fi_right = dp_right/(dt.nanoseconds/S_TO_NS)
        fi_left = dp_left/(dt.nanoseconds/S_TO_NS)

        linear = (self.wheel_radius_*fi_right + self.wheel_radius_*fi_left)/2
        angular = (self.wheel_radius_*fi_right - self.wheel_radius_*fi_left)/self.wheel_separation_

        d_s = (self.wheel_radius_*dp_right + self.wheel_radius_*dp_left)/2
        d_theta = (self.wheel_radius_*dp_right - self.wheel_radius_*dp_left)/self.wheel_separation_
        self.theta_ += d_theta
        self.x_ += d_s*math.cos(self.theta_)
        self.y_ += d_s*math.sin(self.theta_)

        self.get_logger().info("linear: %f, angular: %f"%(linear,angular))
        self.get_logger().info("x: %f, y: %f, theta: %f"%(self.x_,self.y_,self.theta_))

def main(args=None):
    rclpy.init(args=args)
    node = SimpleController() 
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()