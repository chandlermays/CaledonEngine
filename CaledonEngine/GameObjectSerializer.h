/*------------------------------
| File: ComponentTraits.h
| Author: Chandler Mays
------------------------------*/
#pragma once

#include <string>

namespace tinyxml2
{
	class XMLDocument;
	class XMLElement;
}

namespace CE
{
	class Scene;
	class GameObject;	

	class GameObjectSerializer
	{
	public:
		static std::string SerializeScene(const Scene& scene);
		static std::string SerializeGameObject(const Scene& scene, const GameObject* pGameObject);

	private:
		static tinyxml2::XMLElement* SerializeGameObjectElement(tinyxml2::XMLDocument* pDocument, const GameObject* pGameObject);
		static void SerializeComponents(tinyxml2::XMLDocument* pDocument, tinyxml2::XMLElement* pElement, const GameObject* pGameObject);
		static void SerializeChildren(tinyxml2::XMLDocument* pDocument, tinyxml2::XMLElement* pElement, const GameObject* pGameObject);
	};
}