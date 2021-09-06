#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Header.h"
#include <geometry_msgs/PoseStamped.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>

#define NUMBER_OF_ROOMS 1

std::vector<std::string> queue;

visualization_msgs::Marker marker;
visualization_msgs::MarkerArray marker_array;
visualization_msgs::Marker markerTV;
visualization_msgs::MarkerArray markerTV_array;
geometry_msgs::PoseStamped current_goal;
std::string last_goal = "";

ros::Publisher goal_pub;

//std::string rooms[13] = {"Stue 25","Stue 24", "Stue 23", "Stue 22", "Stue 21", "Stue 20B", "Stue 20A", "Stue 19","Stue 18", "Stue 17", "Stue 16", "Stue 15", "Stue 14"};
//double coordinates[13][2] = { {-0.729,-127}, {1.98,-130}, {5.07,-132}, {8.05,-135}, {11.1,-137},{15.5,-141},{17.1,-142},{21.5,-146},{23.6,-156},{26.2,-153},{28.9,-150},{31.5,-148},{34.3,-145}};

std::string rooms[1] = {"Stue 16"};
//New lab coord:
//Quat: {0,0, 0.8478, 0.5303}
double coordinates[1][2] = {{-1.15, -0.344}};
//New lab gang coord:
//1: i enden
//Quat: {0,0, 0.985, 0.169}
//double coordinates[1][2] = {{24.58, -5.56}};
//2: foran indgangen til LAB
//Quat: {0,0, 0.9784, 0.2066}
//double coordinates[1][2] = {{6.40, 1.316}};

void publish_goal()
{
  std::string queueString = queue[0];

  //only publish goal once when button is first in queue
  if (last_goal.compare(queueString.substr(1)) != 0)
  {
    last_goal = queueString.substr(1);
    for(int i = 0; i < NUMBER_OF_ROOMS; i++)
    {
      int pos = queueString.find(rooms[i]);
      if(pos == 1)
      {
        std_msgs::Header header;
        header.stamp = ros::Time::now();
        header.frame_id = "map";
        current_goal.header = header;
        //Pose
        current_goal.pose.position.x = coordinates[i][0];
        current_goal.pose.position.y = coordinates[i][1];
        current_goal.pose.position.z = 0.0;
        //Fixed orientation
        current_goal.pose.orientation.x = 0.0;
        current_goal.pose.orientation.y = 0.0;
        current_goal.pose.orientation.z = 0.8478;
        current_goal.pose.orientation.w = 0.5303;
        goal_pub.publish(current_goal);
      }
    }
  }
}

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  queue.clear();
  queue.push_back(msg->data);
  publish_goal();
}

void checkQueue()
{
  std::string queueString = queue[0];
  for(int i = 0; i < NUMBER_OF_ROOMS; i++)
  {
    if(queueString.find(rooms[i]) != std::string::npos){
      int pos = queueString.find(rooms[i]);
      if(pos == 1)
      {
        marker.action = visualization_msgs::Marker::ADD;
        markerTV.action = visualization_msgs::Marker::ADD;
        marker.id = i;

        marker.text = rooms[i];
        marker.pose.position.x = coordinates[i][0];  //DEFINE
        marker.pose.position.y = coordinates[i][1];   //DEFINE
        marker.pose.position.z = 1;
        marker.color.r = 0.0f;
        marker.color.g = 1.0f;
        marker.color.b = 0.0f;

        marker_array.markers[i] = marker;

        markerTV.id = i;

        markerTV.text = rooms[i];
        markerTV.pose.position.x = coordinates[i][0];
        markerTV.pose.position.y = coordinates[i][1];
        markerTV.pose.position.z = 1.5;

        markerTV_array.markers[i] = markerTV;
      }
      else if(pos > 1 && pos < 10)
      {
        marker.action = visualization_msgs::Marker::ADD;
        markerTV.action = visualization_msgs::Marker::ADD;
        marker.id = i;

        marker.text = rooms[i];
        marker.pose.position.x = coordinates[i][0];  //DEFINE
        marker.pose.position.y = coordinates[i][1];   //DEFINE
        marker.pose.position.z = 1;
        marker.color.r = 0.0f;
        marker.color.g = 0.0f;
        marker.color.b = 1.0f;

        marker_array.markers[i] = marker;

        markerTV.id = i;

        markerTV.text = rooms[i];
        markerTV.pose.position.x = coordinates[i][0];
        markerTV.pose.position.y = coordinates[i][1];
        markerTV.pose.position.z = 1.5;

        markerTV_array.markers[i] = markerTV;
      }
      else{
        marker.action = visualization_msgs::Marker::DELETE;
        markerTV.action = visualization_msgs::Marker::DELETE;
        marker.id = i;
        markerTV.id = i;
        marker_array.markers[i] = marker;
        markerTV_array.markers[i] = markerTV;
      }
    }
    else{
      marker.action = visualization_msgs::Marker::DELETE;
      markerTV.action = visualization_msgs::Marker::DELETE;
      marker.id = i;
      markerTV.id = i;
      marker_array.markers[i] = marker;
      markerTV_array.markers[i] = markerTV;
    }
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "room_interface");
  ros::NodeHandle n;
  ros::Rate r(1);
  queue.push_back("1");

  //Markers setup

  uint32_t shape = visualization_msgs::Marker::CYLINDER;
  uint32_t shapeTV = visualization_msgs::Marker::TEXT_VIEW_FACING;
  marker_array.markers.resize(NUMBER_OF_ROOMS);
  markerTV_array.markers.resize(NUMBER_OF_ROOMS);

  marker.header.frame_id = "/map";
  marker.header.stamp = ros::Time();

  marker.ns = "rooms";

  marker.type = shape;
  marker.action = visualization_msgs::Marker::ADD;

  marker.pose.orientation.x = 0.0;
  marker.pose.orientation.y = 0.0;
  marker.pose.orientation.z = 0.0;
  marker.pose.orientation.w = 1.0;

  marker.scale.x = 0.8;
  marker.scale.y = 0.8;
  marker.scale.z = 0.8;

  marker.color.r = 0.0f;
  marker.color.g = 0.0f;
  marker.color.b = 1.0f;
  marker.color.a = 1.0;

  marker.lifetime = ros::Duration();

  ///////////////////////////////////

  markerTV.header.frame_id = "/map";
  markerTV.header.stamp = ros::Time();

  markerTV.ns = "roomsTV";

  markerTV.type = shapeTV;
  markerTV.action = visualization_msgs::Marker::ADD;

  markerTV.pose.orientation.x = 0.0;
  markerTV.pose.orientation.y = 0.0;
  markerTV.pose.orientation.z = 0.0;
  markerTV.pose.orientation.w = 1.0;

  //markerTV.scale.x = 1.0;
  //markerTV.scale.y = 1.0;
  //markerTV.scale.z = 1.0;

  markerTV.color.r = 1.0f;
  markerTV.color.g = 1.0f;
  markerTV.color.b = 0.0f;
  markerTV.color.a = 1.0;

  markerTV.lifetime = ros::Duration();

  //Markers setup end

  //begin = ros::Time::now();
  ros::Publisher marker_pub = n.advertise<visualization_msgs::MarkerArray>("room_markers", 1);
  goal_pub = n.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 1);
  ros::Subscriber sub = n.subscribe("roomQueue", 10, chatterCallback);
  //ros::spin();

  while(ros::ok())
  {
    checkQueue();
    //Check which rooms are in queue and add marker to array
    //Publish corresponding rooms as markers
    marker_pub.publish(marker_array);
    marker_pub.publish(markerTV_array);
    ros::spinOnce();
    r.sleep();
  }

  return 0;
}
