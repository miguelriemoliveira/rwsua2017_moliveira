#include <iostream>
#include <vector>

//ROS INCLUDES
#include <ros/ros.h>

using namespace std;
using namespace boost;

namespace rwsua2017
{

  class Player
  {
    public:
      ros::NodeHandle n;
      boost::shared_ptr< vector<string> > red_team;
      boost::shared_ptr< vector<string> > green_team;
      boost::shared_ptr< vector<string> > blue_team;
      boost::shared_ptr< vector<string> > my_team;
      boost::shared_ptr< vector<string> > preys_team;
      boost::shared_ptr< vector<string> > hunters_team;

      Player(string argin_name, string argin_team_name = "blue")
      {
        cout << "player name " << argin_name << endl;   
        this->name = argin_name;

        //Create the three teams
        red_team = (boost::shared_ptr<vector <string> >) new vector<string>();
        green_team = (boost::shared_ptr<vector <string> >) new vector<string>();
        blue_team = (boost::shared_ptr<vector <string> >) new vector<string>();

        //ler parametrso com lista de jogadores por equipa
        n.getParam("red", *red_team);
        n.getParam("green", *green_team);
        n.getParam("blue", *blue_team);


        printInfo();

        //set_team_name(argin_team_name);

      }

      void findMyTeam(void)
      {
        for (size_t i = 0; i < red_team->size(); ++i)
        {
          if (red_team->at(i) == name)
          {
            my_team = red_team;
            preys_team = green_team;
            hunters_team = blue_team;
            cout << "I am from team red" << endl;
          }
        }

        for (size_t i = 0; i < green_team->size(); ++i)
        {
          if (green_team->at(i) == name)
          {
            my_team = green_team;
            preys_team = blue_team;
            hunters_team = red_team;
            cout << "I am from team green" << endl;
          }
        }

        for (size_t i = 0; i < blue_team->size(); ++i)
        {
          if (blue_team->at(i) == name)
          {
            my_team = blue_team;
            preys_team = red_team;
            hunters_team = green_team;
            cout << "I am from team blue" << endl;
          }
        }


      }



      void printInfo(void)
      {
        cout << "red team:" << endl; 
        for (size_t i = 0; i < red_team->size(); ++i)
        {
          cout << red_team->at(i) << endl; 
        }

        cout << "green team:" << endl; 
        for (size_t i = 0; i < green_team->size(); ++i)
        {
          cout << green_team->at(i) << endl; 
        }

        cout << "blue team:" << endl; 
        for (size_t i = 0; i < blue_team->size(); ++i)
        {
          cout << blue_team->at(i) << endl; 
        }
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
