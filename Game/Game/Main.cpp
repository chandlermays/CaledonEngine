/*------------------------------
| File: Main.cpp
| Author: Chandler Mays
------------------------------*/
#include "Game.h"

/*----------------------------------------------
| --- Main: Entry point of the application --- |
----------------------------------------------*/
int main()
{
	Game game;

	if (!game.Initialize())
	{
		return -1;
	}

	game.Run();

	return 0;
}