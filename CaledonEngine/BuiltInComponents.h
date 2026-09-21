/*------------------------------
| File: BuiltInComponents.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include <memory>

namespace tinyxml2
{
	class XMLElement;
}

namespace CE
{
	class GameObject;
	class Component;

	class BuiltInComponents
	{
	public:
		static void RegisterAll();																										// Registers all built-in components with the ComponentFactory

	private:
		static std::unique_ptr<Component> CreateSpriteComponentFromXml(GameObject* pGameObject, tinyxml2::XMLElement* pElement);		// Creates a SpriteComponent from an XML element
		static std::unique_ptr<Component> CreateBoxCollider2DFromXml(GameObject* pGameObject, tinyxml2::XMLElement* pElement);			// Creates a BoxCollider2D from an XML element
	};
}