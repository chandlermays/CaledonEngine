/*------------------------------
| File: Main.cpp
| Author: Chandler Mays
------------------------------*/
/*-------------------------------------------------------------------------------------------------
| Entry Point: Game (Standalone Player)
| ------------------------------------
| Lightweight runtime executable for running compiled CaledonEngine games (similar to Unity Player).
|
| Responsibilities:
| - Serves as a Visual Studio startup project for testing standalone performance and release builds.
| - Bypasses all editor UI, docking, and tooling overhead for maximum performance.
| - Loads project configuration and default scene XML files on boot to jump directly into gameplay.
| - Dynamically loads the project's game assembly and executes the main loop.
-------------------------------------------------------------------------------------------------*/
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