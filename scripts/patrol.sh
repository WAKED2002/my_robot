#!/bin/bash
source /opt/ros/jazzy/setup.bash

go() {
  echo "Going to x=$1 y=$2"
  ros2 action send_goal /navigate_to_pose nav2_msgs/action/NavigateToPose \
    "{pose: {header: {frame_id: map}, pose: {position: {x: $1, y: $2, z: 0.0}, orientation: {w: 1.0}}}}"
}

go 2.5 2.5
go -2.5 2.5
go -2.5 -2.5
go 2.5 -2.5
go 0.0 0.0
echo "Patrol finished"