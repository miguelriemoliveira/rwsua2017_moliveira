#include <iostream>
#include <vector>


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

}
