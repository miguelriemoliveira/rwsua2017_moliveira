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
      TransformBroadcaster br;
      Transform t1;

      MyPlayer(string argin_name, string argin_team_name): Player(argin_name, argin_team_name)
    {
      //Subscribe tyo the make_a_play_message
      sub = n.subscribe("/make_a_play", 1000, &MyPlayer::makeAPlayCallback, this);

        t1.setOrigin( tf::Vector3(1, 1, 0.0) );
        Quaternion q;
        q.setRPY(0, 0, 0);
        t1.setRotation(q);
        br.sendTransform(tf::StampedTransform(t1, ros::Time::now(), "map", name));

      cout << "Initialized MyPlayer" << endl;
    };


      void makeAPlayCallback(const rwsua2017_msgs::MakeAPlay::ConstPtr& msg)
      {
        cout << "received a make a play msg with max_displacement = " << msg->max_displacement << endl;


        //Definicao dos angulos de rotação e valores de translação 
        //DEVERIA SER CALCULADO PELA AI DO SISTEMA
        float turn_angle = M_PI/10;
        float displacement = 0.5;

        //Compute the new reference frame
        tf::Transform t_mov;
        Quaternion q;
        q.setRPY(0, 0, turn_angle);
        t_mov.setRotation(q);
        t_mov.setOrigin( Vector3(displacement , 0.0, 0.0) );

        tf::Transform t = t1  * t_mov;
        //Send the new transform to ROS
        br.sendTransform(StampedTransform(t, ros::Time::now(), "/map", name));
      }

      vector<string> teammates;



  };
}


int main(int argc, char **argv)
{
  cout << "Hello world" << endl;           

  ros::init(argc, argv, "player_moliveira");


  rwsua2017::MyPlayer myplayer("player_moliveira", "green");

  cout << "name = " << myplayer.name << endl;
  cout << "team name = " << myplayer.get_team_name() << endl;

  myplayer.teammates.push_back("fsilva");
  myplayer.teammates.push_back("vsilva");

  //ciclo for tipico em c
  //int i;
  //for (i=0; i < 5; i = i + 1)

  cout << "teammates:" << endl; 
  for (size_t i = 0; i < myplayer.teammates.size(); ++i)
  {
    cout << myplayer.teammates[i] << endl; 
  }

  ros::spin();

  return 1;                                                      
}






