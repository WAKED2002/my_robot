# my_robot

A small drivable robot simulated in Gazebo and controlled with ROS 2 Jazzy.
It maps a room, localizes itself, and patrols several goals with Nav2.
A camera on the robot is used to read images and find colors.

## What it does

- Simulates the robot and a room in Gazebo (`urdf/robot.urdf`, `worlds/room.sdf`)
- Builds a map with SLAM Toolbox and navigates with Nav2 (`config/`, `maps/`)
- Patrols a list of goals with a shell script (`scripts/patrol.sh`)
- C++ nodes in `src/`: pose reader, go-to-goal, circle driver, battery monitor, camera reader, color finder

## Requirements

- Ubuntu 24.04 (tested in WSL2)
- ROS 2 Jazzy
- Gazebo Harmonic with `ros_gz_bridge`
- Nav2 and SLAM Toolbox

## Build

    cd ~/ros2_ws
    colcon build --packages-select my_robot
    source install/setup.bash

## Run

    ros2 launch my_robot sim_nav.launch.py

In a second terminal, start the patrol:

    bash src/my_robot/scripts/patrol.sh

## Status

Work in progress. I am learning robotics step by step; next steps are a C++ PID controller for heading control and a demo GIF.

