/*------------------------------
| File: Main.cpp
| Author: Chandler Mays
------------------------------*/
#include "Game.h"

#include <iostream>

struct AllocationMetrics
{
	int32_t TotalAllocated = 0;
	int32_t TotalFreed = 0;

	int32_t CurrentUsage() { return TotalAllocated - TotalFreed; }
};

static AllocationMetrics s_AllocationMetrics;

void* operator new(size_t size)
{
	s_AllocationMetrics.TotalAllocated += size;

	return malloc(size);
}

void operator delete(void* memory, size_t size)
{
	s_AllocationMetrics.TotalFreed += size;

	free(memory);
}

static void PrintMemoryUsage()
{
	std::cout << "Memory Usage: " << s_AllocationMetrics.CurrentUsage() << " bytes.\n";
}

/*----------------------------------------------
| --- Main: Entry point of the application --- |
----------------------------------------------*/
int main()
{
	PrintMemoryUsage();
	{
		Game game;

		PrintMemoryUsage();
		if (!game.Initialize())
		{
			return -1;
		}

		PrintMemoryUsage();
		game.Run();

		std::cout << "Shutdown.\n";
	} // The 'game' object goes out of scope here, triggering Game::~Game()

	PrintMemoryUsage();
	return 0;
}