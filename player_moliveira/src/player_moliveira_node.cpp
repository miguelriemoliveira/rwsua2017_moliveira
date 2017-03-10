//ATD includes
#include <iostream>
#include <vector>

//ROS INCLUDES
#include <ros/ros.h>

//Local includes
#include <rwsua2017_libs/player.h>
#include <rwsua2017_msgs/MakeAPlay.h>


using namespace std;

namespace rwsua2017
{

  class MyPlayer: public Player
  {
    public:
      ros::NodeHandle n;
      int a;
      ros::Subscriber sub;

    MyPlayer(string argin_name, string argin_team_name): Player(argin_name, argin_team_name)
    {
      sub = n.subscribe("/make_a_play", 1000, &MyPlayer::makeAPlayCallback, this);

      a = 7;
      int b = 5;

      cout << "Initialized MyPlayer" << endl;
    };

      void makeAPlayCallback(const rwsua2017_msgs::MakeAPlay::ConstPtr& msg)
      {
        cout << "received a make a play msg with max_displacement = " << msg->max_displacement << endl;
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






