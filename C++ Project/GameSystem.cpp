#include "GameSystem.h"
#include "Level.h"
#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include <ios>     
#include <istream>
#include <limits> 
#include <io.h>
#include <conio.h>
#include <stdio.h>

GameSystem::GameSystem(string level)
{

	_level.load(level);//Loads the map
	_level.print();//Prints the map
	_level.setWeather_Treasure();//Sets the weather,treasure and ports of the map
}

void GameSystem::PlayGame()
{
	char w;
	while (true)
	{
		while (!_kbhit())
		{
			_level.setWeather_Treasure();//Sets random weather and treasure
			_level.print();//Prints the map
			_level.W_Damage();//Do weather damage
			_level.Give_tres();//Sets the treasure that was found in the current tile
			_level.AtPort();//Boat at a port
			_level.updateBoats();//Moves the boats	
			_level.end_game();//Checks if the game must end
			Sleep(800);//Stops the run for 800 milliseconds
		}

		w = _getch();
		if (w == 'p' || w == 'P')
			_level.pause_menu();//Pause menu
	}
}
