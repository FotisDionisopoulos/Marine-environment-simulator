#pragma once
#include <vector>
#include <string>
#include "Boats.h"
#include <conio.h>
#include <thread>
#include <chrono>

using namespace std;

class Level
{
public:
	Level();
	void load(string filename);
	void print();
	void updateBoats();
	char getTile(int x, int y);
	int get_i(int x, int y);
	void setTile(int x, int y, char tile);
	void setWeather_Treasure();
	void W_Damage();
	void Give_tres();
	void AtPort();
	void battle(int i,int i2, int x,int y,int z,int w);
	void end_game();

	//AI commands
	char getMove(int i, char b, int px, int py);

	//Pause menu
	void pause_menu();
	void ship_info();
	void pos_info();
	void boat_type();
	void general_info();
	vector <int> tres_sort;
	void add_ship();
	void remove_ship();
	void add_port();
	void remove_port();
	void add_th();
	void remove_th();
	void prt_m();
	void prt_dmg();
	void prt_p_dmg();
	void prt_m_g();
	void prt_r_g();
	
private:
	vector <string> _levelData;//Map's vector
	vector <Boats> _boats;//Vector where boats are stored
	vector <Boats> _pir_dmg;//Pirate damage
	vector <vector <int> > _weather;//Weather values
	vector <vector <bool> > _treasure;//Treasure values
	vector <vector <bool>>  _is_port;//It is or not a port
	vector <vector <int>>  _port_hp;//If it is a port ,then here is it's health points
	vector <int>  _temp1;//Used to store other vectors 
	vector <bool>  _temp2;//Used to store other vectors 
	vector <bool>  _temp3;//Used to store other vectors 
	vector <int>  _temp4;//Used to store other vectors 
	vector <int>  _distance;//Distance between ships
	vector <int>  _id;//The index of a ship
	vector <int> _pause_st;//Stores data for the pause menu

	void processMove(int enemyIndex, int targetX, int targetY);
};

