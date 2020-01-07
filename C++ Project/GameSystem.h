#pragma once
#include "Level.h"
#include <string>

using namespace std;

class GameSystem
{
public:
	GameSystem(string level);
	void PlayGame();
	
private:
	Level _level;
};

