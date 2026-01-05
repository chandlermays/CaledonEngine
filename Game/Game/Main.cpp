#include "../CaledonEngine/Engine/EngineManager.h"
#include <Core/GameObject.h>
#include <Components/SpriteComponent.h>

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
	testObject.GetComponent<CE::SpriteComponent>();

	return 0;
}