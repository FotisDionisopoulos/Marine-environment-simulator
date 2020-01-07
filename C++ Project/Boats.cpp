#include "Boats.h"
#include "Level.h"
#include <string>
#include <random>
#include <ctime>

using namespace std;

Boats::Boats(char name, char tile, int attack, int curr_hp, int max_hp, int speed, int tres,int moves,int s_dmg)
{
	_name = name;
	_tile = tile;
	_attack = attack;
	_curr_hp = curr_hp;
	_max_hp = max_hp;
	_speed = speed;
	_tres = tres;
	_moves = moves;
	_s_dmg = s_dmg;
}

//Sets the position of the enemy
void Boats::setPosition(int x, int y)
{
	_x = x;
	_y = y;
}

//Gets the position of the enemy(by reference)
void Boats::getPosition(int &x, int &y)
{
	x = _x;
	y = _y;
}


//Gets the current tile
char Boats::getTile()
{
	return _tile;
}

//Gets the treasure
int Boats::getTres()
{
	return _tres;
}

//Gets the _max_hp
int Boats::getMaxHp()
{
	return _max_hp;
}

//Gets the name
char Boats::getName()
{
	return _name;
}

//Gets the speed of the boat
void  Boats::getSpeed(int &speed)
{
	speed= _speed;
}

//Gets the hp of the boat
int Boats::get_curr_hp()
{ 
	return _curr_hp;
}

//Sets the new hp of the boat due to the weather damage
void Boats::set_curr_hp(int w)
{
	_curr_hp -= 1;
}

//If the hp of the boat is <=0 ,then it's removed from the vector
int Boats::CheckDeath()
{
	if (_curr_hp <= 0)
	{
		return 0;
	}
	else return 1;
}

//Sets the treasure that was found in the current tile
void Boats::set_tres(int g)
{
	_tres += g;
}

//Sets the new hp(port repairs the boat)
void Boats::set_Hp(float w)
{
	int c = w*_curr_hp + _curr_hp;
	if (c > _max_hp)
	{
		_curr_hp = _max_hp;
	}
	else _curr_hp = c;
}

//Adds extra gold for the repair of the ship from the port
void Boats::set_Port_Tres()
{
	_tres += _tres + 90;
}

//Gets the attack
int Boats::get_attack()
{
	return _attack;
}

//Sets the new hp from the battle
void Boats::set_battle_hp(int h)
{
	_curr_hp = h;
}

//Stores the moves
void Boats::set_moves()
{
	_moves += _speed;
}

//Gets the moves
int  Boats::get_moves()
{
	return _moves;
}

//Sets oveall damage
void Boats::set_s_dmg(int d)
{
	_s_dmg +=d;
}

//Gets overall damage
int  Boats::get_s_dmg()
{
	return _s_dmg;
}