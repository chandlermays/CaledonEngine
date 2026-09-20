/*------------------------------
| File: Main.cpp
| Author: Chandler Mays
------------------------------*/
#include "Game.h"

#include "CaledonEngine/Utilities/LeakDetector.h"

/*----------------------------------------------
| --- Main: Entry point of the application --- |
----------------------------------------------*/
int main()
{
	LeakDetector::Start();

	Game game;

	if (!game.Initialize())
	{
		return -1;
	}

	game.Run();

	LeakDetector::Report();

	return 0;
}