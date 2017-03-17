//ATD includes
#include <iostream>
#include <vector>

//ROS INCLUDES
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <visualization_msgs/Marker.h>

//Local includes
#include <rwsua2017_libs/player.h>
#include <rwsua2017_msgs/MakeAPlay.h>


using namespace std;
using namespace boost;
using namespace tf;
using namespace ros;

namespace rwsua2017
{

  class MyPlayer: public Player
  {
    public:

      //PROPPERTIES
      Subscriber sub;
      tf::TransformListener listener;
      TransformBroadcaster br;
      ros::Publisher vis_pub;
      bool hunting;

      MyPlayer(string argin_name, string argin_team_name): Player(argin_name, argin_team_name)
    {
      //Subscribe tyo the make_a_play_message
      sub = n.subscribe("/make_a_play/cat", 1000, &MyPlayer::makeAPlayCallback, this);

      vis_pub = n.advertise<visualization_msgs::Marker>( "/bocas", 0 );


      Transform t1;
      t1.setOrigin( tf::Vector3(randNumber(),randNumber(), 0.0) );
      Quaternion q;
      q.setRPY(0, 0, 0);
      t1.setRotation(q);
      br.sendTransform(tf::StampedTransform(t1, ros::Time::now(), "map", name));

      hunting = true;

      ROS_INFO_STREAM("Initialized MyPlayer");
    };

      double randNumber(){
        struct timeval t1;
        gettimeofday(&t1,NULL);
        srand(t1.tv_usec);
        double x =((((double)rand()/(double)RAND_MAX)*2 -1)*5);

        return x;
      }

      float getAngleTo(string player_name, float time_to_wait = 0.1)
      {
        tf::StampedTransform trans;
        ros::Time now = Time(0); //get the latest transform received

        try
        {
          listener.waitForTransform(name, player_name, now, Duration(time_to_wait));
          listener.lookupTransform(name, player_name, now, trans);
        }
        catch (tf::TransformException ex){
          ROS_ERROR("%s",ex.what());
          ros::Duration(0.01).sleep();
        }

        float x = trans.getOrigin().x();
        float y = trans.getOrigin().y();

        return atan2(y,x);

      }

      tf::StampedTransform getPose(float time_to_wait = 0.1)
      {
        tf::StampedTransform trans;
        ros::Time now = Time(0); //get the latest transform received

        try
        {
          listener.waitForTransform("map", name, now, Duration(time_to_wait));
          listener.lookupTransform("map", name, now, trans);
        }
        catch (tf::TransformException ex){
          ROS_ERROR("%s",ex.what());
          ros::Duration(0.01).sleep();
        }

        return trans;
      }

      float getDistanceToPlayer(string player_name, float time_to_wait = 0.1)
      {
        tf::StampedTransform trans;
        ros::Time now = Time(0); //get the latest transform received

        try
        {
          listener.waitForTransform(name, player_name, now, Duration(time_to_wait));
          listener.lookupTransform(name, player_name, now, trans);
        }
        catch (tf::TransformException ex){
          ROS_ERROR("%s",ex.what());
          ros::Duration(0.01).sleep();
          return 10000;
        }

        float x = trans.getOrigin().x();
        float y = trans.getOrigin().y();
        return sqrt(x*x + y*y);
      }

      string getClosestPlayer(vector<string> players)
      {
        
        string closest_player = "";
        float min_distance = 100000;

        for (size_t i=0; i < players.size(); ++i)
        {
          float d = getDistanceToPlayer(players[i]);
          if (d < min_distance) 
          {
            min_distance = d;
            closest_player = players[i];
          }
        }

        if (closest_player == "")
        {
          ROS_WARN("Could not find closest player");
        }

        return closest_player;
      }

      void makeAPlayCallback(const rwsua2017_msgs::MakeAPlay::ConstPtr& msg)
      {
        ROS_INFO_STREAM("received a make a play msg with max_displacement = " << msg->max_displacement);

        //Definicao dos angulos de rotação e valores de translação 
        string closest_prey = getClosestPlayer(msg->green_alive);
        string closest_hunter = getClosestPlayer(msg->blue_alive);

        float closest_prey_d = getDistanceToPlayer(closest_prey);
        float closest_hunter_d = getDistanceToPlayer(closest_hunter);

        //Change values to obtain histeresis behaviour
        if (hunting)
        {
          closest_prey_d = closest_prey_d * 0.8;
        }
        else
        {
          closest_hunter_d = closest_hunter_d * 0.8; 
        }

        if (closest_hunter_d > closest_prey_d)
        {
          hunting = false;
        }
        else
        {
          hunting = true;
        }



        string anchor_player = "jferreira";
        float turn_angle;
       
        = getAngleTo(player_to_hunt);

        float displacement = msg->max_displacement;

        ROS_INFO("Hunting player %s", player_to_hunt.c_str());

        ROS_WARN("This is a warning");
        ROS_ERROR("This is an error");

        //if (msg->blue_alive.size() > 0) //there are alive hunters, let's flee
        //{
          ////find the closest hunter ... cannot do this now so will just flee from vsilva
          //turn_angle = getAngleTo("vsilva") + 5*M_PI/6;
          //ROS_INFO("Fleeing from vsilva");
        //} 
        //else
        //{
          ////just use the default values 
        //}


       

        //move my player
        move(displacement, turn_angle, msg->max_displacement, M_PI/30);

        //enviar boca
        visualization_msgs::Marker marker;
        marker.header.frame_id = name;
        marker.header.stamp = ros::Time();
        marker.ns = name;
        marker.id = 0;
        marker.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
        marker.action = visualization_msgs::Marker::ADD;
        marker.pose.position.x = 0; marker.pose.position.y = 0.4; marker.pose.position.z = 0;
        marker.pose.orientation.x = 0.0; marker.pose.orientation.y = 0.0; marker.pose.orientation.z = 0.0;
        marker.pose.orientation.w = 1.0;
        marker.scale.z = 0.4;
        marker.color.a = 1.0; // Don't forget to set the alpha!
        marker.color.r = 0.3;
        marker.color.g = 0.3;
        marker.color.b = 0.3;
        marker.frame_locked = 1;
        marker.lifetime = ros::Duration(1);
        marker.text = "nao gosto de bocas!";
        vis_pub.publish(marker);
      }

      void move(float displacement, float turn_angle, float max_displacement, float max_turn_angle)
      {
        //Saturate tunr angle
        double max_t =  max_turn_angle;
        if (turn_angle > max_t) turn_angle = max_t;
        else if (turn_angle < -max_t) turn_angle = -max_t;

        //Saturate displacement
        if (displacement > max_displacement)
        {
          displacement = max_displacement;
        }

        //Compute the new reference frame
        tf::Transform t_mov;
        Quaternion q;
        q.setRPY(0, 0, turn_angle);
        t_mov.setRotation(q);
        t_mov.setOrigin( Vector3(displacement , 0.0, 0.0) );

        tf::Transform t = getPose()  * t_mov;
        //Send the new transform to ROS
        br.sendTransform(StampedTransform(t, ros::Time::now(), "/map", name));
      }

      vector<string> teammates;



  };
}


int main(int argc, char **argv)
{
  cout << "Hello world" << endl;           

  ros::init(argc, argv, "moliveira");


  rwsua2017::MyPlayer myplayer("moliveira", "green");

  cout << "name = " << myplayer.name << endl;
  //cout << "team name = " << myplayer.get_team_name() << endl;

  for (size_t i = 0; i < myplayer.teammates.size(); ++i)
  {
    cout << myplayer.teammates[i] << endl; 
  }

  ros::spin();

  return 1;                                                      
}






