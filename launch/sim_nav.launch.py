import re

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import ExecuteProcess, IncludeLaunchDescription, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue

BASE = '/home/khalid/ros2_ws/src/my_robot'


def generate_launch_description():
    # Read the robot file and remove the XML comments
    with open(BASE + '/urdf/robot.urdf') as f:
        robot_xml = f.read()
    robot_xml = re.sub(r'<!--.*?-->', '', robot_xml, flags=re.DOTALL)

    nav2_dir = get_package_share_directory('nav2_bringup')

    # Terminal 1: Gazebo
    gazebo = ExecuteProcess(
        cmd=['gz', 'sim', '-r', BASE + '/worlds/room.sdf'],
        output='screen')

    # Terminal 2: put the robot in the world (after 8 seconds)
    spawn = TimerAction(period=8.0, actions=[
        Node(package='ros_gz_sim', executable='create',
             arguments=['-world', 'room',
                        '-file', BASE + '/urdf/robot.urdf',
                        '-name', 'my_robot',
                        '-z', '0.1'],
             output='screen')])

    # Terminal 3: bridge
    bridge = Node(
        package='ros_gz_bridge', executable='parameter_bridge',
        arguments=[
            '/cmd_vel@geometry_msgs/msg/Twist]gz.msgs.Twist',
            '/odom@nav_msgs/msg/Odometry[gz.msgs.Odometry',
            '/scan@sensor_msgs/msg/LaserScan[gz.msgs.LaserScan',
            '/image_raw@sensor_msgs/msg/Image[gz.msgs.Image',
            '/tf@tf2_msgs/msg/TFMessage[gz.msgs.Pose_V',
            '/clock@rosgraph_msgs/msg/Clock[gz.msgs.Clock'],
        output='screen')

    # Terminal 4: robot_state_publisher
    state_pub = Node(
        package='robot_state_publisher', executable='robot_state_publisher',
        parameters=[{
            'use_sim_time': True,
            'robot_description': ParameterValue(robot_xml, value_type=str)}],
        output='screen')

    # Terminal 5: Nav2 (after 15 seconds)
    nav2 = TimerAction(period=15.0, actions=[
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(
                nav2_dir + '/launch/bringup_launch.py'),
            launch_arguments={
                'map': BASE + '/maps/room.yaml',
                'params_file': BASE + '/config/nav2_params.yaml',
                'use_sim_time': 'true'}.items())])

    # Terminal 7: RViz (last, after 60 seconds)
    rviz = TimerAction(period=60.0, actions=[
        Node(package='rviz2', executable='rviz2',
             arguments=['-d', nav2_dir + '/rviz/nav2_default_view.rviz'],
             parameters=[{'use_sim_time': True}],
             output='screen')])

    return LaunchDescription([gazebo, spawn, bridge, state_pub, nav2, rviz])