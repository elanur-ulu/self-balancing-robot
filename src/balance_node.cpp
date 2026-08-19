#include <chrono>
#include <memory>
#include <cmath>
#include <algorithm>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "geometry_msgs/msg/vector3.hpp"
#include "std_msgs/msg/float64.hpp"
#include "sensor_msgs/msg/joint_state.hpp"

using namespace std::chrono_literals;

class BalanceBotController : public rclcpp::Node {
public:
    BalanceBotController() : Node("balance_bot_controller") {
        kp_ = 22.0;
        ki_ = 4.5;
        kd_ = 3.2;

        target_pitch_ = 0.0;
        current_pitch_ = 0.0;
        pitch_velocity_ = 0.0;
        integral_error_ = 0.0;
        last_error_ = 0.0;
        external_force_ = 0.0;
        wheel_pos_ = 0.0;

        cmd_vel_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
        pitch_pub_ = this->create_publisher<std_msgs::msg::Float64>("/robot_pitch", 10);
        motor_pub_ = this->create_publisher<std_msgs::msg::Float64>("/motor_cmd", 10);
        joint_pub_ = this->create_publisher<sensor_msgs::msg::JointState>("/joint_states", 10);

        force_sub_ = this->create_subscription<geometry_msgs::msg::Vector3>(
            "/apply_force", 10,
            std::bind(&BalanceBotController::force_callback, this, std::placeholders::_1));

        timer_ = this->create_wall_timer(10ms, std::bind(&BalanceBotController::control_loop, this));

        RCLCPP_INFO(this->get_logger(), "3B Model Destekli Denge Robotu Hazir!");
    }

private:
    void force_callback(const geometry_msgs::msg::Vector3::SharedPtr msg) {
        external_force_ += msg->x;
        RCLCPP_WARN(this->get_logger(), ">>> DARBE ALINDI! Kuvvet: %.2f N", msg->x);
    }

    void control_loop() {
        double dt = 0.01;

        // PID Hesabı
        double error = target_pitch_ - current_pitch_;
        integral_error_ += error * dt;
        integral_error_ = std::clamp(integral_error_, -2.0, 2.0);
        double derivative_error = (error - last_error_) / dt;

        double control_output = (kp_ * error) + (ki_ * integral_error_) + (kd_ * derivative_error);
        last_error_ = error;

        // Fizik Simülasyonu
        double gravity = 9.81 * std::sin(current_pitch_);
        double damping = -0.35 * pitch_velocity_;
        double angular_accel = gravity + external_force_ + (control_output * 3.0) + damping;

        external_force_ = 0.0;

        pitch_velocity_ += angular_accel * dt;
        current_pitch_ += pitch_velocity_ * dt;
        wheel_pos_ += control_output * dt * 5.0; // Tekerlek dönme hareketi

        // 1. Topic Yayınları
        auto pitch_msg = std_msgs::msg::Float64();
        pitch_msg.data = current_pitch_ * (180.0 / M_PI);
        pitch_pub_->publish(pitch_msg);

        auto motor_msg = std_msgs::msg::Float64();
        motor_msg.data = control_output;
        motor_pub_->publish(motor_msg);

        // 2. 3B RViz Eklem Durumları (JointState)
        auto joint_state = sensor_msgs::msg::JointState();
        joint_state.header.stamp = this->now();
        joint_state.name = {"pitch_joint", "left_wheel_joint", "right_wheel_joint"};
        joint_state.position = {current_pitch_, wheel_pos_, wheel_pos_};
        joint_pub_->publish(joint_state);
    }

    double kp_, ki_, kd_;
    double target_pitch_, current_pitch_, pitch_velocity_;
    double integral_error_, last_error_, external_force_;
    double wheel_pos_;

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub_;
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr pitch_pub_;
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr motor_pub_;
    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_pub_;
    rclcpp::Subscription<geometry_msgs::msg::Vector3>::SharedPtr force_sub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<BalanceBotController>());
    rclcpp::shutdown();
    return 0;
}
