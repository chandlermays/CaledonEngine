#include "../CaledonEngine/Engine/EngineManager.h"

/*----------------------------------------------
| --- Main: Entry point of the application --- |
----------------------------------------------*/
int main()
{
	CE::EngineManager engineManager;
	if (!engineManager.Initialize())
	{
		return -1;
	}
	engineManager.Run();

	return 0;
}