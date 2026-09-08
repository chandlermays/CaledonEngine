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
		static void RegisterAll();		// Registers every Engine-owned Component type with ComponentFactory

	private:
		static Component* CreateSpriteComponentFromXml(GameObject* pGameObject, tinyxml2::XMLElement* pElement);
		static Component* CreateBoxCollider2DFromXml(GameObject* pGameObject, tinyxml2::XMLElement* pElement);
	};
}