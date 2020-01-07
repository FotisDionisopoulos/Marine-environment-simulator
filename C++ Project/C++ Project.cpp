#include <iostream>
#include <cstdlib>
#include <string>
#include <conio.h>
#include "GameSystem.h"

using namespace std;

int main()
{
	GameSystem gameSystem("Map.txt");

	gameSystem.PlayGame();

	return 0;
}