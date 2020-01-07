#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <algorithm> 
#include "Level.h"
#include "Boats.h"
#include "GameSystem.h"

using namespace std;

Level::Level()
{

}

void Level::load(string filename)
{
	int randMove,x,y,randx,randy;
	
	//Level loading
	ifstream file;

	file.open(filename);
	if (file.fail())
	{
		perror(filename.c_str());
		system("PAUSE");
		exit(1);
	}

	string line;

	while (getline(file, line))
	{
		_levelData.push_back(line);
	}

	file.close();
	
	static default_random_engine randomEngine((unsigned int)time(NULL));
	uniform_int_distribution<int> moveRollx(2, 75);
	uniform_int_distribution<int> moveRolly(2, 18);
	
	//Random positions for the ships
	randx = moveRollx(randomEngine);
	randy = moveRolly(randomEngine);
	setTile(randx, randy, 'P');	

	randx = moveRollx(randomEngine);
	randy = moveRolly(randomEngine);
	setTile(randx, randy, 'E');

	randx = moveRollx(randomEngine);
	randy = moveRolly(randomEngine);
	setTile(randx, randy, 'R');

	randx = moveRollx(randomEngine);
	randy = moveRolly(randomEngine);
	setTile(randx, randy, 'M');

	//Level processing
	//Loop through the game board and process the level
	for (int i = 0; i < _levelData.size(); i++) {
		//Loop through each letter in row i
		for (int j = 0; j < _levelData[i].size(); j++) {
			//This is the current tile we are looking at
			//char tile = _levelData[i][j];
			_pause_st.push_back(0);
			char tile = _levelData[i][j];
			//Check the tile to see what we should do
			switch (tile) {
			case 'P': //Pirate
				_boats.push_back(Boats('P', tile, 5, 20, 20, 1, 0, 0, 0));
				_boats.back().setPosition(j, i);
				break;
			case 'R': //Repair ship
				_boats.push_back(Boats('R', tile, 4,25,25, 2, 0,0,0));
				_boats.back().setPosition(j, i);
				break;
			case 'M': //Merchant vessel
				_boats.push_back(Boats('M', tile, 4,40,40, 2, 0,0,0));
				_boats.back().setPosition(j, i);
				break;
			case 'E': //Exploration ships
				_boats.push_back(Boats('E', tile, 4, 15,15, 2, 0,0,0));
				_boats.back().setPosition(j, i);
				break;
			case '#': //All the tiles that dont need extra processing, in this case, walls and air
			case '.':
				//Doesnt need to do anthing, just break.
				break;
			}
		}
	}
}

//Prints the map
void Level::print()
{
	
	for (int i = 0; i < 50; i++)
	{
		printf("\n");
	}
	for (int i = 0; i < _levelData.size(); i++)
	{
		printf("%s\n", _levelData[i].c_str());
	}
}

//Moves the ships
void Level::updateBoats()
{
	char AIMove;
	int enemyX;
	int enemyY;
	int s,in_s;
	char b;
	//Loop through the boats vector
	for (int i = 0; i < _boats.size(); i++) {
		_boats[i].set_moves();
		in_s = _boats.size();
		//Get the AI movement from the boat
		b = _boats[i].getName();
		//Get the current boat position
		_boats[i].getPosition(enemyX, enemyY);
		AIMove = getMove(i,b,enemyX,enemyY);	
			//Get the spped of the boat
		if (in_s > _boats.size())
		{
			break;
		}
			_boats[i].getSpeed(s);
			//Process the AI move command
			switch (AIMove) {
			case 'w': //up
				processMove(i, enemyX, enemyY - s);
				break;
			case 's': //down
				processMove(i, enemyX, enemyY + s);
				break;
			case 'a': //left
				processMove(i, enemyX - s, enemyY);
				break;
			case 'd': //right
				processMove(i, enemyX + s, enemyY);
				break;
			}
			
		}
	
	}

//GetMove gives the right move foe every ship
char Level::getMove(int index,char b, int myx, int myy)
{
	int dx, x,minx,index2;
	int dy, y,miny,j=0;
	int adx;
	int ady;
	int min;
	_id.clear();
	_distance.clear();


	static default_random_engine randomEngine((unsigned int)time(NULL));
	uniform_int_distribution<int> moveRoll(0, 3);

	int randMove = moveRoll(randomEngine);

		//If it's a pirate ,it attacks the nearest ship
		if (b == 'P')
		{
			for (int i = 0; i < _boats.size(); i++)
			{
			_boats[i].getPosition(x, y);
			if (x != myx && y != myy)
			{
				_boats[i].getPosition(x, y);
				dx = x - myx;
				dy = y - (myy);
				adx = abs(dx);
				ady = abs(dy);
				_distance.push_back(adx + ady);
				_id.push_back(i);
			}
			}
			if (_distance.size()>0)
			{
				int min = _distance[0];
				index2 = _id[0];
				_boats[_id[0]].getPosition(minx, miny);
				for (int i = 1; i < _distance.size(); i++)
				{
					if (_distance[i] < min)
					{
						min = _distance[i];
						index2 = i;
						_boats[_id[i]].getPosition(minx, miny);
						dx = minx - myx;
						dy = miny - (myy);
						adx = abs(dx);
						ady = abs(dy);
					}
				}
				if (min <=5)
				{
					randMove = 4;
					cout << "BATTLE"<<endl;
					battle(index, _id[index2], myx, myy, minx, miny);
				}
			}
		}
		//If it's a merchant ship and there is port it increases treasure
		else if (b == 'M')
		{
			if (_is_port[myy][myx] == true && _port_hp[myy][myx] > 0)
			{
				_boats[index].set_Port_Tres();
				_pause_st[index] += 90;
			}
		}
		//If it's a repair ship ,it repair a near ship
		else if (b == 'R')
		{
			for (int i = 0; i < _boats.size(); i++)
			{
				_boats[i].getPosition(x, y);
				if (x !=myx && y != myy)
				{
					_boats[i].getPosition(x, y);
					dx = x - myx;
					dy = y - (myy);
					adx = abs(dx);
					ady = abs(dy);
					_distance.push_back(adx + ady);
					_id.push_back(i);
				}
			}
			if (_distance.size()>0)
			{
				int min = _distance[0];
				index2 = _id[0];
				_boats[_id[0]].getPosition(minx, miny);
				for (int i = 0; i < _distance.size(); i++)
				{
					if (_distance[i] < min)
					{
						min = _distance[i];
						index2 = i;
						_boats[_id[i]].getPosition(minx, miny);
						dx = minx - myx;
						dy = miny - (myy);
						adx = abs(dx);
						ady = abs(dy);
					}

				}
				if (min <= 4)
				{
					if (_boats[_id[index2]].getTres() >= 100)
					{
						_boats[_id[index2]].set_Hp(0.2);
						_boats[index].set_tres(50);
						_pause_st[index] += 100;
					}
				}
			}
		}
		else if (b == 'E')
		{
			for (int i = 0; i < _boats.size(); i++)
			{
				
				char w = _boats[i].getName();

				if (w == 'P')
				{
					_boats[i].getPosition(x, y);
					dx = x - myx;
					dy = y  - (myy);
					adx = abs(dx);
					ady = abs(dy);
					_distance.push_back(adx + ady);
					_id.push_back(i);
				}
			}
			if (_distance.size()>0)
			{
				int min = _distance[0];
				index2 = _id[0];
				_boats[_id[0]].getPosition(minx, miny);
				for (int i = 0; i < _distance.size(); i++)
				{
					if (_distance[i] < min)
					{
						min = _distance[i];
						index2 = i;
						_boats[_id[i]].getPosition(minx, miny);
						dx = minx - myx;
						dy = miny - (myy);
						adx = abs(dx);
						ady = abs(dy);

					}
				}

				if (min <= 10)
				{
					//RUUUUUUUUUUUUUUUUUUUUUUN!!!
					//Moving along X axis
					if (adx > ady) {
						//if pirate is left, move right. Otherwise move left
						if (dx > 0) {
							printf("To ploio'E'pige aristera apo peirati\n");
							return 'a';
						}
						else {
							printf("To ploio'E'pige deksia apo peirati\n");
							return 'd';
						}
					}
					else { //Move along Y axis
						if (dy > 0) {
							printf("To ploio'E'pige panw apo peirati\n");
							return 'w';
						}
						else {
							printf("To ploio'E'pige katw apo peirati\n");
							return 's';
						}
					}
				}
			}
		}
	


	switch (randMove){
	case 0: //left
		printf("To ploio'%c'pige aristera\n", getTile(myx, myy));
		return 'a';
	case 1: //up
		printf("To ploio'%c'pige panw\n", getTile(myx, myy));
		return 'w';
	case 2: //down
		printf("To ploio'%c'pige katw\n", getTile(myx, myy));
		return 's';
	case 3: //right
		printf("To ploio'%c'pige deksia\n", getTile(myx, myy));
		return 'd';
	case 4: //No movement
		printf("To ploio'%c'emeine sthn thesi tou\n", getTile(myx, myy));
		break;
	}
}

//Changes the tiles based on the target position
void Level::processMove(int enemyIndex, int targetX, int targetY)
{
	int enemyX;
	int enemyY;

	_boats[enemyIndex].getPosition(enemyX, enemyY);

	//Get the tile the enemy wants to move to
	char moveTile = getTile(targetX, targetY);

	//Process the tile
	switch (moveTile) {
	case '.': //if its a dot, we move there
		_boats[enemyIndex].setPosition(targetX, targetY);
		setTile(enemyX, enemyY, '.');
		setTile(targetX, targetY, _boats[enemyIndex].getTile());
		break;
	case '#': //if its a wall, we break!
		break;
	default:
		//Battle();
		break;

	}
}

//Gets the tile
char Level::getTile(int x, int y)
{
	return _levelData[y][x];
}

void Level::setTile(int x, int y, char tile)
{
	_levelData[y][x] = tile;
}

//Sets weather and treasure for the tiles
void Level::setWeather_Treasure()
{
	int rand1,rand2,rand3,rand4;
	static default_random_engine randomEngine(time(NULL));
	uniform_int_distribution<int> moveRoll1(0, 10);
	uniform_int_distribution<int> moveRoll2(0, 1);
	uniform_int_distribution<int> moveRoll3(0, 1);
	//Loop through the game board and set the weather , treasure and ports
	for (int i = 0; i < _levelData.size(); i++) {
	for (int j = 0; j <_levelData[0].length(); j++) {
			//Set the weather
			rand1 = moveRoll1(randomEngine);	
			_temp1.push_back(rand1);
			//Set the treasure(bool)
			rand2 = moveRoll2(randomEngine);
			if (rand2 == 1)
			{
				_temp2.push_back(true);
			}
			else _temp2.push_back(false);
			//Set the ports
			rand3 = moveRoll3(randomEngine);
			if (rand3== 1)
			{
				_temp3.push_back(true);
				//Set hp of the ports
				_temp4.push_back(100);
			}
			else
			{
				_temp3.push_back(false);
				_temp4.push_back(0);
			}
		}

		_weather.push_back(_temp1);
		_treasure.push_back(_temp2);	
		_is_port.push_back(_temp3);
		_port_hp.push_back(_temp4);
		_temp1.clear();
		_temp2.clear();
		_temp3.clear();
		_temp4.clear();

	}

}

//Do damage if the weather value is over 9
void Level::W_Damage()
{
	int x, y;
	int w;
	for (int i = 0; i < _boats.size(); i++) {
		_boats[i].getPosition(x, y);
		if (_weather[y ][x ] >= 9)
				{
					printf("To ploio sti thesi %d,%d epathe zimia apo ton kairo\n", x, y);
					_boats[i].set_s_dmg(1);
					_boats[i].set_curr_hp(_weather[y][x]);
					if (_boats[i].CheckDeath() == 0)
					{
						setTile(x,y,'.');
						_boats[i] = _boats.back();
						_boats.pop_back();
						i--;
					}
				}
			}
}

//Give treasure if it's available
void Level::Give_tres()
{
	int x, y, rand;
	static default_random_engine randomEngine(time(NULL));
	uniform_int_distribution<int> moveRoll1(50, 200);
	for (int i = 0; i < _boats.size(); i++) {
		_boats[i].getPosition(x, y);
		if (_treasure[y][x ] == true)
		{
			rand = moveRoll1(randomEngine);
			_boats[i].set_tres(rand);
			_treasure[y ][x] == false;
		}
	}
}

//When a ship meets a port...
void Level::AtPort()
{
	int x, y, rand;
	char boat;
	for (int i = 0; i < _boats.size(); i++)
	{
		_boats[i].getPosition(x, y);
		boat=getTile(x, y);
		if (_is_port[y ][x ] == true)
		{
				switch (boat)
			{
				case 'P':
					_port_hp[y][x] -= 0.2;
					break;
				case 'M':
				case 'E':
				case 'R':
					if (_port_hp[y][x] > 0)
					{
						_boats[i].set_Hp(1.2);
					}
					break;
			}
		}
	}

}

//Battle with the pirate
void Level::battle(int indexP,int indexb,int x, int y,int mx, int my)
{
	int a1, a2;
	int h1, h2;
	while ((_boats[indexP].get_curr_hp()>0) && (_boats[indexb].get_curr_hp()>0))
	{
		//Pirate turn
		a1 = _boats[indexP].get_attack();
		h1 = _boats[indexb].get_curr_hp();
		h1 -= a1;
		_pause_st[indexP] += a1;
		_boats[indexb].set_s_dmg(a1);
		_boats[indexb].set_battle_hp(h1);
		if (h1<=0)
		{
			//Boat dead	
			setTile(mx, my, '.');
			_boats[indexb] = _boats.back();
			_boats.pop_back();
			return;
		} 
		//Boat turn
		a2 = _boats[indexb].get_attack();
		h2 = _boats[indexP].get_curr_hp();
		h2 -= a2;
		_boats[indexP].set_s_dmg(a2);
		_boats[indexP].set_battle_hp(h2);
		if (h2 <= 0)
		{
			//Pirate dead
			setTile(x, y, '.');	
			_boats[indexP] = _boats.back();
			_boats.pop_back();
			return;
		}
	}
}

//Pause menu
void Level::pause_menu()
{
	int ci;
	do{
		cout << "\n*********************";
		cout << "\n1.Plirofories ploioy";
		cout << "\n2.Plhrofories thesis";
		cout << "\n3.Typos ploioy";
		cout << "\n4.Genikes plhrofories";
		cout << "\n5.Prosthesi ploiou";
		cout << "\n6.Afairesi ploiou";
		cout << "\n7.Prosthesi limaniou";
		cout << "\n8.Afairesi limaniou";
		cout << "\n9.Prosthesi thisaurou";
		cout << "\n10.Afairesi thisaurou";
		cout << "\n11.Synolikes theseis metakinisis";
		cout << "\n12.Sinoliki zimia";
		cout << "\n13.Sinoliki zimia pou exei prokalesie peiratis";
		cout << "\n14.Sinolikos xrusos pou exei kerdisei episkeyastiko ploio";
		cout << "\n15.Sinolikos xrusos pou exei kerdisei apo ta limania emporiko ploio";
		cout << "\n16.Eksodos apo to menu";
		cout << "\n*********************\n";
		cin >> ci;

		switch (ci)
		{
		case 1:
			ship_info();
			break;
		case 2:
			pos_info();
			break;
		case 3:
			boat_type();
			break;
		case 4:
			general_info();
			break;
		case 5:
			add_ship();
			return;
			break;
		case 6:
			remove_ship();
			break;
		case 7:
			add_port();
			break;
		case 8:
			remove_port();
			break;
		case 9:
			add_th();
			break;
		case 10:
			remove_th();
			break;
		case 11:
			prt_m();
			break;
		case 12:
			prt_dmg();
			break;
		case 13:
			prt_p_dmg();
			break;
		case 14:
			prt_r_g();
			break;
		case 15:
			prt_m_g();
			break;
		}

	} while (ci == 1 || ci == 2 || ci == 3 || ci == 4 || ci == 5 || ci == 6 || ci == 7 || ci == 8 || ci == 9 || ci == 10 || ci == 11 || ci == 12 || ci == 13 || ci == 14 || ci == 15);
}

//Ship information
void Level::ship_info()
{
	int x, y, w,z, f,s;
	//Prints available coordinates
	cout << "Oi diathesimes syntetagmenes ploion einai:\n";
	for (int i = 0; i < _boats.size(); i++)
	{
		_boats[i].getPosition(f, z);
		cout << "(" << f<< "," << z << ")" << endl;
	}
	cout << "Dwste syntetagmenes(x,y):\n";
	cin >> x >> y;
	if (y <= _levelData.size() && x <= _levelData[0].length()){
		
			if (getTile(x, y) == 'P' || getTile(x, y) == 'E' || getTile(x, y) == 'M' || getTile(x, y) == 'R')
			{

				//Get the position of the boat in the vector
				w = get_i(x, y);
				//Print boat properties
				_boats[w].getSpeed(s);
				printf("Xrusos:%d\n", _boats[w].getTres());
				printf("Megisti antoxi:%d\n", _boats[w].getMaxHp());
				printf("Trexousa antoxi:%d\n", _boats[w].get_curr_hp());
				printf("Taxytita:%d\n", s);
				printf("Dynami:%d\n", _boats[w].get_attack());
			} 
			else if (getTile(x, y) == '.' || getTile(x, y) == '#')
			{
				cout << "Dwsate lathos syntetagmenes.\n";
			}
	}
}
	
//Position information
void Level::pos_info()
{
	int x, y;

	cout << "Dwste syntetagmenes(x,y):\n";
	cout << "max X:" << _levelData[0].length()<<endl;
	cout << "max Y:" << _levelData.size()<<endl;
	cin >> x >> y;
	printf("Exei xruso:");
	if (_treasure[x][y] == true)
	{
		printf("nai\n");
	}
	else printf("oxi\n");
	printf("Kairos:%d",_weather[x][y]);
	printf("\nEinai limani:");
	if (_is_port[x][y] == true)
	{
		printf("nai, kai i trexoysa antoxi toy einai %d\n",_port_hp[x][y]);
	}
	else printf("oxi\n");

}

//Information about boat type
void Level::boat_type()
{
	char c;
	int j=0, s;
	cout << "Dwste ton typo toy ploiou:";
	cin >> c;
	for (int i = 0; i < _boats.size(); i++)
	{
		if (c == _boats[i].getTile())
			j++;
	}
	cout << "Yparxoyn " << j << " ploia typou " << c << " kai gia ayta isxyoyn:\n";
	if (j > 0)
	{
		for (int i = 0; i < _boats.size(); i++)
		{
			if (c == _boats[i].getTile())
			{
				_boats[i].getSpeed(s);
				printf("\nXrusos:%d\n", _boats[i].getTres());
				printf("Megisti antoxi:%d\n", _boats[i].getMaxHp());
				printf("Trexousa antoxi:%d\n", _boats[i].get_curr_hp());
				printf("Taxytita:%d\n", s);
				printf("Dynami:%d\n\n", _boats[i].get_attack());


			}
			
		}
	}	else	cout << "Den yparxoyn ploia typou" << c;
}

//General Information about the simulation
void Level::general_info()
{
	tres_sort.clear();
	for (int i = 0; i < _boats.size(); i++)
	{
		tres_sort.push_back(_boats[i].getTres());
	}
	sort(tres_sort.begin(), tres_sort.end());
	cout << "H katataksi ton ploion me vasi ton thysayro toys einai(auksousa):\n";
	for (int i = 0; i < _boats.size(); i++)
	{
		for (int j = 0; j < _boats.size(); j++)
		{
			if (_boats[j].getTres() == tres_sort[i])
			{
				cout << "Ploio typou " << _boats[j].getName() << " me thysayro " << tres_sort[i]<<endl;
				break;
			}
		}
	}
}

//Finds the index of the boat using the coordinates
int Level::get_i(int x, int y)
{
	int w, z;
	for (int i = 0; i < _boats.size(); i++)
	{
		_boats[i].getPosition(w, z);
		if ((x ==w) && (y == z))
		{
			return i;
		}
	}
}

//Adds a ship
void Level::add_ship()
{
	int x, y;
	char c;
	cout << "Dwste syntetagmenes(x,y):\n";
	cout << "min X: 0 max X :" << _levelData[0].length()-6 << endl;
	cout << "min Y: 0 max Y :" << _levelData.size() -6<< endl;
	cin >> x >> y;
	if (x <= _levelData[0].length() - 6 && y <= _levelData.size() - 6)
	{
		x += 2;
		y += 2;
		if (getTile(x, y) == '.')
		{
			cout << "Dwste ton typo toy ploiou(P/M/E/R):\n";
			cin >> c;
			if (c == 'P')
			{
				_boats.push_back(Boats(c, c, 5, 20, 20, 1, 0, 0, 0));
				_boats.back().setPosition(x, y);
			}

			else{
				_boats.push_back(Boats(c, c, 5, 20, 20, 2, 0, 0, 0));
				_boats.back().setPosition(x, y);
			}
		}
		else 	{
			cout << "Dwsate lathos syntetagmenes\n";
			add_ship();
		}
	
	} else
	{
		cout << "Dwsate lathos syntetagmenes,dwste ksana syntetagmenes.\n";
		add_ship();
	}
}

//Removes ship
void Level::remove_ship()
{
	int x, y,w,f,z;
	//Prints available coordinates
	cout << "Oi diathesimes syntetagmenes ploion einai:\n";
	for (int i = 0; i < _boats.size(); i++)
	{
		_boats[i].getPosition(f, z);
		cout << "(" << f << "," << z << ")" << endl;
	}
	cout << "Dwste syntetagmenes(x,y) gia afairesi ploiou:\n";
	cin >> x >> y;
	w = get_i(x, y);
	_boats[w] = _boats.back();
	_boats.pop_back();
	setTile(x, y, '.');

}

//Adds port 
void Level::add_port()
{
	int x, y;
	char c;
	cout << "Dwste syntetagmenes(x,y):\n";
	cout << "max X:" << _levelData[0].length()-1 << endl;
	cout << "max Y:" << _levelData.size()-1 << endl;
	cin >> x >> y;
	if (x <= _levelData[0].length() - 1 && y <= _levelData.size() - 1)
	{
		if (_is_port[y][x] == false)
		{
			_is_port[y][x] = true;
			_port_hp[y][x] = 100;
		}
		else 	{
			cout << "Yparxei hdh limani sto sigkekrimeno simeio,dwste ksana syntetagmenes.\n";
			add_port();
		}
	}
	else
	{
		cout << "Dwsate lathos syntetagmenes,dwste ksana syntetagmenes.\n";
		add_port();
	}
}

//Removes port
void Level::remove_port()
{
	int x, y;
	char c;
	cout << "Dwste syntetagmenes(x,y):\n";
	cout << "max X:" << _levelData[0].length()-1 << endl;
	cout << "max Y:" << _levelData.size() -1<< endl;
	cin >> x >> y;
	if (x <= _levelData[0].length() - 1 && y <= _levelData.size() - 1)
	{
		if (_is_port[y][x] == true)
		{
			_is_port[y][x] = false;
			_port_hp[y][x] = 0;
		}
		else 	{
			cout << "Den yparxei limani sto sigkekrimeno simeio,dwste ksana syntetagmenes.\n";
			remove_port();
		}
	}
	else
	{
		cout << "Dwsate lathos syntetagmenes,dwste ksana syntetagmenes.\n";
		remove_port();
	}

}

//Add treasure
void Level::add_th()
{
	int x, y;
	char c;
	cout << "Dwste syntetagmenes(x,y):\n";
	cout << "max X:" << _levelData[0].length() - 1 << endl;
	cout << "max Y:" << _levelData.size() - 1 << endl;
	cin >> x >> y;
	if (x <= _levelData[0].length() - 1 && y <= _levelData.size() - 1)
	{
		if (_treasure[y][x] == false)
		{
			_treasure[y][x] = true;
			_port_hp[y][x] = 100;
		}
		else 	{
			cout << "Yparxei hdh xrusos sto sigkekrimeno simeio,dwste ksana syntetagmenes.\n";
			add_th();
		}
	}
	else{
		cout << "Dwsate lathos syntetagmenes,dwste ksana syntetagmenes.\n";
	add_th();
}
}

//Removes treasure
void Level::remove_th()
{
	int x, y;
	char c;
	cout << "Dwste syntetagmenes(x,y):\n";
	cout << "max X:" << _levelData[0].length() - 1 << endl;
	cout << "max Y:" << _levelData.size() - 1 << endl;
	cin >> x >> y;
	if (x <= _levelData[0].length() - 1 && y <= _levelData.size() - 1)
	{
		if (_treasure[y][x] == true)
		{
			_treasure[y][x] = false;
		}
		else 	{
			cout << "Deb yparxei xrusos sto sigkekrimeno simeio,dwste ksana syntetagmenes.\n";
			remove_th();
		}
	}
	else{
		cout << "Dwsate lathos syntetagmenes,dwste ksana syntetagmenes.\n";
		remove_th();
	}
}

//Prints all the moves of a ship
void Level::prt_m()
{
	int x, y, w, f, z;
	//Prints available coordinates
	cout << "Oi diathesimes syntetagmenes ploion einai:\n";
	for (int i = 0; i < _boats.size(); i++)
	{
		_boats[i].getPosition(f, z);
		cout << "(" << f << "," << z << ")" << endl;
	}
	cout << "Dwste syntetagmenes(x,y):\n";
	cin >> x >>y;
	w = get_i(x, y);
	cout << "Oi synolikes theseis einai: " << _boats[w].get_moves();
}

//Prints overall damage that the ship has taken
void Level::prt_dmg()
{
	int x, y, w, f, z;
	//Prints available coordinates
	cout << "Oi diathesimes syntetagmenes ploion einai:\n";
	for (int i = 0; i < _boats.size(); i++)
	{
		_boats[i].getPosition(f, z);
		cout << "(" << f << "," << z << ")" << endl;
	}
	cout << "Dwste syntetagmenes(x,y):\n";
	cin >> x >> y;
	w = get_i(x, y);
	cout << "H synoliki zimia pou exei dextei einai: " << _boats[w].get_s_dmg();
}

//Prints the damage that the pirate caused
void Level::prt_p_dmg()
{
	int x, y, w, f, z;
	//Prints available coordinates
	cout << "Oi diathesimes syntetagmenes ploion einai:\n";
	for (int i = 0; i < _boats.size(); i++)
	{
		if (_boats[i].getName() == 'P')
		{
			_boats[i].getPosition(f, z);
			cout << "(" << f << "," << z << ")" << endl;
		}
	}
	cout << "Dwste syntetagmenes(x,y):\n";
	cin >> x >> y;
	w = get_i(x, y);
	cout << "H synoliki zimia pou exei prokalesei einai: " << _pause_st[w];
}

//Prints the amount of gold that the repair ship has earned from the  repairs
void Level::prt_r_g()
{
	int x, y, w, f, z;
	//Prints available coordinates
	cout << "Oi diathesimes syntetagmenes ploion einai:\n";
	for (int i = 0; i < _boats.size(); i++)
	{
		if (_boats[i].getName() == 'R')
		{
			_boats[i].getPosition(f, z);
			cout << "(" << f << "," << z << ")" << endl;
		}
	}
	cout << "Dwste syntetagmenes(x,y):\n";
	cin >> x >> y;
	w = get_i(x, y);
	cout << "O synolikos xrusos pou exei kerdisei to episkeyastiko ploio einai : " << _pause_st[w];
}

//Prints the amount of gold that the merchant ship has earned from the  ports
void Level::prt_m_g()
{
	int x, y, w, f, z;
	//Prints available coordinates
	cout << "Oi diathesimes syntetagmenes ploion einai:\n";
	for (int i = 0; i < _boats.size(); i++)
	{
		if (_boats[i].getName() == 'M')
		{
			_boats[i].getPosition(f, z);
			cout << "(" << f << "," << z << ")" << endl;
		}
	}
	cout << "Dwste syntetagmenes(x,y):\n";
	cin >> x >> y;
	w = get_i(x, y);
	cout << "O synolikos xrusos pou exei kerdisei to emporiko ploio apo limania einai : " << _pause_st[w];
}

//Checks if the simulation has to end
void Level::end_game()
{
	if (_boats.size() == 0)
	{
		printf("Den ypaxoun alla ploia ston xarti.Telos prosomoiosis.\n");
		system("pause");
		exit(0);
	}
	for (int i = 0; i < _boats.size(); i++)
	{
		if (_boats[i].getTres() >= 1000000000000)
		{
			printf("Sigkentrothike aparaititi posotita xrusou.Telos prosomoiosis.\n");
			system("pause");
			exit(0);
		}
	}
}