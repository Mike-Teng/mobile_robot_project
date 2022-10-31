#include <ros/ros.h>
#include <std_msgs/Int64.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/ByteMultiArray.h>
#include <std_msgs/String.h>

ros::Publisher num_pub;
bool arduino_return_state = true;

void state_cb(const std_msgs::Int64::ConstPtr& ptr){
  ROS_INFO_STREAM("Num received from Arduino is: " << ptr->data);
  arduino_return_state = true;
}

int main(int argc, char** argv){
  ros::init(argc, argv, "cp3");
  ros::NodeHandle nh("");
  std_msgs::String action;
  // action.data.resize(2);
  action_pub = nh.advertise<std_msgs::String>("action", 10);
  ros::Subscriber state_sub = nh.subscribe("state", 10, &state_cb);

  try
  {
    ROS_INFO("[Check Point 3]: Initializing node");

    while(ros::ok){
      if (arduino_return_state || true){
        // ROS_INFO_STREAM("user's right is");
        std::cout << "user's right is " ;
        std::cin >> num.data[0];
        std::cout << "user's left is " ;
        std::cin >> num.data[1];
        std::cout << "----------------------\n" ;
        // ROS_INFO_STREAM("Num sent to Arduino is: " << num.data);

        num_pub.publish(action);
        arduino_return_state = false;
      }
      ros::spinOnce();
    }
      
  }
  catch (const char* s)
  {
    ROS_FATAL_STREAM("[Check Point 3]: " << s);
  }
  catch (...)
  {
    ROS_FATAL_STREAM("[Check Point 3]: Unexpected error");
  }

  return 0;
}
