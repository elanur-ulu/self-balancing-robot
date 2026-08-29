# Two-Wheeled Self-Balancing Robot (ROS 2)

This project involves modeling a two-wheeled self-balancing robot, defining its kinematic structure, and implementing PID-based closed-loop control within the **ROS 2** environment.

---

## Project Summary

The robot is designed to maintain its balance by preserving the tilt angle (pitch) of its body along the vertical axis. Angular position and velocity data obtained from the IMU are processed, and the necessary torque/speed commands are transmitted to the motors via a **PID control algorithm**.

---

## Technologies & Tools Used

* **Operating System:** Linux (Ubuntu / WSL 2)
* **Middleware:** ROS 2
* **Modeling:** URDF (Unified Robot Description Format)
* **Programming Language:** C++17
* **Algorithm:** PID Control (Proportional-Integral-Derivative)

---

## Package Structure

```text
balance_bot_control/
├── CMakeLists.txt
├── package.xml
├── src/
│   └── balance_node.cpp    # Balance and PID control node
└── urdf/
    └── balance_bot.urdf     # Mechanical and visual URDF model of the robot
