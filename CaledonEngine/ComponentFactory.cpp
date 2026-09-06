/*------------------------------
| File: ComponentFactory.cpp
| Author: Chandler Mays
------------------------------*/
#include "ComponentFactory.h"

#include "Systems/Rendering/Components/SpriteComponent.h"
#include "Systems/Physics/Components/BoxCollider2D.h"
#include "Systems/Rendering/Shapes/Square.h"
#include "Systems/Rendering/Shapes/Circle.h"
#include "Systems/Rendering/Shapes/Triangle.h"
#include "Systems/Rendering/Shapes/Capsule.h"
#include "Systems/Rendering/Sprite.h"
#include "Core/GameObject.h"
#include "Utilities/ThirdParty/tinyxml2.h"

#include <unordered_set>

using namespace tinyxml2;

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
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
	else if (componentID == "BoxCollider2D")
	{
		AddBoxCollider2DComponent(pGameObject, pElement);
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
			"SpriteComponent",
			"BoxCollider2D"
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


/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*-------------------------------------------------------------------------
| --- AddSpriteComponent: Attach a Sprite Component to the GameObject --- |
-------------------------------------------------------------------------*/
void CE::ComponentFactory::AddSpriteComponent(GameObject* pGameObject, tinyxml2::XMLElement* pElement)
{
	const char* pSpritesheet = pElement->Attribute("spritesheet");
	int frameWidth = pElement->IntAttribute("frameWidth");
	int frameHeight = pElement->IntAttribute("frameHeight");
	float scale = pElement->FloatAttribute("scale");
	const char* pShapeType = pElement->Attribute("shape");

	SpriteComponent* pSpriteCmp = nullptr;

	if (pSpritesheet && frameWidth > 0 && frameHeight > 0)
	{
		// Texture-backed sprite, loaded from a spritesheet
		pSpriteCmp = new SpriteComponent(pSpritesheet, frameWidth, frameHeight, scale);

		XMLElement* pColor = pElement->FirstChildElement("Color");
		if (pColor != nullptr)
		{
			unsigned char r = static_cast<unsigned char>(pColor->UnsignedAttribute("r"));
			unsigned char g = static_cast<unsigned char>(pColor->UnsignedAttribute("g"));
			unsigned char b = static_cast<unsigned char>(pColor->UnsignedAttribute("b"));
			unsigned char a = static_cast<unsigned char>(pColor->UnsignedAttribute("a"));
			pSpriteCmp->SetColor(r, g, b, a);		// Tint applied on top of the texture
		}
	}
	else if (pShapeType)
	{
		// Primitive shape sprite (no texture) — <Color> sets the shape's own color directly
		pSpriteCmp = new SpriteComponent();

		Color shapeColor = Color::White();
		XMLElement* pColor = pElement->FirstChildElement("Color");
		if (pColor != nullptr)
		{
			shapeColor.r = static_cast<unsigned char>(pColor->UnsignedAttribute("r"));
			shapeColor.g = static_cast<unsigned char>(pColor->UnsignedAttribute("g"));
			shapeColor.b = static_cast<unsigned char>(pColor->UnsignedAttribute("b"));
			shapeColor.a = static_cast<unsigned char>(pColor->UnsignedAttribute("a"));
		}

		bool isFilled = pElement->BoolAttribute("filled", true);
		int width = pElement->IntAttribute("width", 100);
		int height = pElement->IntAttribute("height", 100);

		std::string shapeType = pShapeType;
		std::unique_ptr<Shape> pShape;

		if (shapeType == "Square")
		{
			pShape = std::make_unique<Square>(shapeColor, width, isFilled);
		}
		else if (shapeType == "Circle")
		{
			pShape = std::make_unique<Circle>(shapeColor, pElement->IntAttribute("radius", width / 2), isFilled);
		}
		else if (shapeType == "Triangle")
		{
			pShape = std::make_unique<Triangle>(shapeColor, width, height, isFilled);
		}
		else if (shapeType == "Capsule")
		{
			pShape = std::make_unique<Capsule>(shapeColor, width, height, isFilled);
		}

		if (pShape)
		{
			pSpriteCmp->SetSprite(Sprite::CreateFromShape(std::move(pShape)));
		}
	}
	else
	{
		// No sprite content specified — a blank SpriteComponent that can be assigned one later
		pSpriteCmp = new SpriteComponent();
	}

	pGameObject->AddComponent(pSpriteCmp);
}

/*---------------------------------------------------------------------------------------
| --- AddBoxCollider2DComponent: Attach a BoxCollider2D Component to the GameObject --- |
---------------------------------------------------------------------------------------*/
void CE::ComponentFactory::AddBoxCollider2DComponent(GameObject* pGameObject, tinyxml2::XMLElement* pElement)
{
	BoxCollider2D* pCollider = new BoxCollider2D();

	const char* pSize = pElement->Attribute("size");
	if (pSize)
	{
		Vector2f size = Vector2f::One();
		sscanf_s(pSize, "%f,%f", &size.x, &size.y);
		pCollider->SetSize(size);
	}

	const char* pOffset = pElement->Attribute("offset");
	if (pOffset)
	{
		Vector2f offset = Vector2f::Zero();
		sscanf_s(pOffset, "%f,%f", &offset.x, &offset.y);
		pCollider->SetOffset(offset);
	}

	pCollider->SetEdgeRadius(pElement->FloatAttribute("edgeRadius", 0.0f));
	pCollider->SetTrigger(pElement->BoolAttribute("isTrigger", false));

	pGameObject->AddComponent(pCollider);
}

/*------------------------------------------------------------------------------
| --- GetRegistry: Returns the registry of Game-defined component creators --- |
------------------------------------------------------------------------------*/
std::unordered_map<std::string, CE::ComponentFactory::CreatorFunc>& CE::ComponentFactory::GetRegistry()
{
	static std::unordered_map<std::string, CreatorFunc> registry;
	return registry;
}