#pragma once
#include <string>

using namespace std;

class Boats
{
public:
	Boats(char name, char tile, int attack, int curr_hp, int max_hp, int speed,int tres,int moves,int s_dmg);

	//Setters
	void setPosition(int x, int y);
	void set_curr_hp(int w);
	void set_tres(int g);
	void set_Hp(float w);
	void set_battle_hp(int h);
	void set_Port_Tres();
	void set_moves();
	void set_s_dmg(int d);


	//Getters 
	void getPosition(int &x, int &y);
	char getTile();
	void getSpeed(int &speed);
	int get_curr_hp();
	int get_attack();
	int getMaxHp();
	int getTres();
	char getName();
	int get_moves();
	int  get_s_dmg();

	int attack();
	int CheckDeath();
	int takeDamage(int attack);

private:
	char _name;
	char _tile;
	int _attack;
	int _curr_hp;
	int _max_hp;
	int _speed;
	int _tres;
	bool _type;
	int _max_hpP;
	int _moves;
	int _s_dmg;
	int _dmg;
	int _i;

	//Position properties
	int _x;
	int _y;

};
