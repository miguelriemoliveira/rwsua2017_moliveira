#include <iostream>
#include <vector>

//indent your code!

using namespace std;

namespace rwsua2017
{

  class Player
  {
    public:

      Player(string argin_name, string argin_team_name = "blue")
      {
        cout << "player name " << argin_name << endl;   
        this->name = argin_name;
        set_team_name(argin_team_name);

      }

      string name;

      void set_team_name(string argin_team_name)
      {
        if (argin_team_name == "red" || argin_team_name == "green" || argin_team_name == "blue")
          this->team_name = argin_team_name;
        else
          cout << "Error incorrect team name" << endl;
      }

      void set_team_name(void) {set_team_name("red");}
      string get_team_name(void){return this->team_name;};

      private:
        string team_name;

  };


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






