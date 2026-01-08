#include "CaledonEngine/Engine/EngineManager.h"
#include "CaledonEngine/Scene/SceneManager.h"
#include "CaledonEngine/Core/GameObject.h"
#include "CaledonEngine/Core/Scene.h"

/*----------------------------------------------
| --- Main: Entry point of the application --- |
----------------------------------------------*/
int main()
{
	CE::EngineManager& engineManager = CE::EngineManager::GetInstance();
	if (!engineManager.Initialize())
	{
		return -1;
	}

	CE::Scene testScene;
	CE::GameObject testObject;

	testScene.AddGameObject(&testObject);
	engineManager.GetSceneManager()->AddScene(&testScene);

	engineManager.Run();

	return 0;
}