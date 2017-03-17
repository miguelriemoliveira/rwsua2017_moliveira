//ATD includes
#include <iostream>
#include <vector>

//ROS INCLUDES
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>

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

      MyPlayer(string argin_name, string argin_team_name): Player(argin_name, argin_team_name)
    {
      //Subscribe tyo the make_a_play_message
      sub = n.subscribe("/make_a_play/cat", 1000, &MyPlayer::makeAPlayCallback, this);

      Transform t1;
      t1.setOrigin( tf::Vector3(randNumber(),randNumber(), 0.0) );
      Quaternion q;
      q.setRPY(0, 0, 0);
      t1.setRotation(q);
      br.sendTransform(tf::StampedTransform(t1, ros::Time::now(), "map", name));

      cout << "Initialized MyPlayer" << endl;
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


      void makeAPlayCallback(const rwsua2017_msgs::MakeAPlay::ConstPtr& msg)
      {
        cout << "received a make a play msg with max_displacement = " << msg->max_displacement << endl;


        //Definicao dos angulos de rotação e valores de translação 
        //DEVERIA SER CALCULADO PELA AI DO SISTEMA
        //float turn_angle = getAngleTo(preys_team->at(1));
        float turn_angle = getAngleTo("jferreira");

        float displacement = msg->max_displacement;

        double max_t =  (M_PI/30);
        if (turn_angle > max_t) turn_angle = max_t;
        else if (turn_angle < -max_t) turn_angle = -max_t;

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






