#pragma once

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
		static void RegisterAll();																							// Registers all built-in components with the ComponentFactory

	private:
		static Component* CreateSpriteComponentFromXml(GameObject* pGameObject, tinyxml2::XMLElement* pElement);			// Creates a SpriteComponent from an XML element
		static Component* CreateBoxCollider2DFromXml(GameObject* pGameObject, tinyxml2::XMLElement* pElement);				// Creates a BoxCollider2D from an XML element
	};
}