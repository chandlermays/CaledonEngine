#include "CaledonEngine/Engine/EngineManager.h"
#include "CaledonEngine/Core/GameObject.h"

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

	CE::GameObject testObject;

	return 0;
}