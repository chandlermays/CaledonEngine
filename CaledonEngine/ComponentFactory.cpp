#include "ComponentFactory.h"
#include "CaledonEngine/Systems/Rendering/Components/SpriteComponent.h"

#include "CaledonEngine/Core/GameObject.h"
#include "CaledonEngine/Utilities/ThirdParty/tinyxml2.h"

#include <unordered_set>

using namespace tinyxml2;

/*-------------------------------------
| --- Public Function Definitions --- |
-------------------------------------*/
/*----------------------------------------------------------------
| --- CreateComponent: Create a Component for the GameObject --- |
----------------------------------------------------------------*/
void CE::ComponentFactory::CreateComponent(GameObject* pGameObject, const std::string& componentID, tinyxml2::XMLElement* pElement)
{
	if (componentID == "SpriteComponent")
	{
		AddSpriteComponent(pGameObject, pElement);
		return;
	}

	// Fall back to any component types the Game project has registered
	auto& registry = GetRegistry();
	auto it = registry.find(componentID);
	if (it != registry.end())
	{
		Component* pComponent = it->second(pGameObject, pElement);
		if (pComponent)
		{
			pGameObject->AddComponent(pComponent);
		}
	}
}

/*--------------------------------------------------------------------------
| --- IsEngineComponent: Check if the Component is an Engine Component --- |
--------------------------------------------------------------------------*/
bool CE::ComponentFactory::IsEngineComponent(const std::string& componentID)
{
	static const std::unordered_set<std::string> engineComponents = {
			"SpriteComponent"
	};

	return engineComponents.find(componentID) != engineComponents.end();
}

/*--------------------------------------------------------------------------------------------------
| --- RegisterComponent: Registers a Game-defined component type so it can be created from XML --- |
--------------------------------------------------------------------------------------------------*/
void CE::ComponentFactory::RegisterComponent(const std::string& componentID, CreatorFunc creator)
{
	GetRegistry()[componentID] = std::move(creator);
}


/*--------------------------------------
| --- Private Function Definitions --- |
--------------------------------------*/
/*-------------------------------------------------------------------------
| --- AddSpriteComponent: Attach a Sprite Component to the GameObject --- |
-------------------------------------------------------------------------*/
void CE::ComponentFactory::AddSpriteComponent(GameObject* pGameObject, tinyxml2::XMLElement* pElement)
{
	const char* pSpritesheet = pElement->Attribute("spritesheet");
	int frameWidth = pElement->IntAttribute("frameWidth");
	int frameHeight = pElement->IntAttribute("frameHeight");
	float scale = pElement->FloatAttribute("scale");

	SpriteComponent* pSpriteCmp = (pSpritesheet && frameWidth > 0 && frameHeight > 0)
		? new SpriteComponent(pSpritesheet, frameWidth, frameHeight, scale)
		: new SpriteComponent();

	XMLElement* pColor = pElement->FirstChildElement("Color");
	if (pColor != nullptr)
	{
		unsigned char r = static_cast<unsigned char>(pColor->UnsignedAttribute("r"));
		unsigned char g = static_cast<unsigned char>(pColor->UnsignedAttribute("g"));
		unsigned char b = static_cast<unsigned char>(pColor->UnsignedAttribute("b"));
		unsigned char a = static_cast<unsigned char>(pColor->UnsignedAttribute("a"));
		pSpriteCmp->SetColor(r, g, b, a);
	}

	XMLElement* pImage = pElement->FirstChildElement("Image");
	
	//...

	pGameObject->AddComponent(pSpriteCmp);
}

/*------------------------------------------------------------------------------
| --- GetRegistry: Returns the registry of Game-defined component creators --- |
------------------------------------------------------------------------------*/
std::unordered_map<std::string, CE::ComponentFactory::CreatorFunc>& CE::ComponentFactory::GetRegistry()
{
	static std::unordered_map<std::string, CreatorFunc> registry;
	return registry;
}