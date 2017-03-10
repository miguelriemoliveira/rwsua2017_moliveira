#include <iostream>
#include <vector>

#include <rwsua2017_libs/player.h>

//test include rws message
#include <rwsua2017_msgs/MakeAPlay.h>

//indent your code!

using namespace std;

namespace rwsua2017
{

  class MyPlayer: public Player
  {
    public:

    MyPlayer(string argin_name, string argin_team_name): Player(argin_name, argin_team_name)
    {
      cout << "Initialized MyPlayer" << endl;
    };
  
    vector<string> teammates;
  
  };
}


int main()
{
  cout << "Hello world" << endl;           

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


  return 1;                                                      
}






