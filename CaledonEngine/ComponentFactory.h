/*------------------------------
| File: ComponentFactory.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include <string>
#include <unordered_map>
#include <functional>

namespace tinyxml2
{
	class XMLElement;
}

namespace CE
{
	class GameObject;
	class Component;

	class ComponentFactory
	{
	public:
		using CreatorFunc = std::function<Component* (GameObject*, tinyxml2::XMLElement*)>;									// Signature for a Game-defined component creator

	private:
		void AddSpriteComponent(GameObject* pGameObject, tinyxml2::XMLElement* pElement);									// Attach a Sprite Component to the GameObject
		void AddBoxCollider2DComponent(GameObject* pGameObject, tinyxml2::XMLElement* pElement);							// Attach a BoxCollider2D Component to the GameObject)
		static std::unordered_map<std::string, CreatorFunc>& GetRegistry();													// Returns the registry of Game-defined component creators

	public:
		void CreateComponent(GameObject* pGameObject, const std::string& componentID, tinyxml2::XMLElement* pElement);		// Create a Component for the GameObject
		static bool IsEngineComponent(const std::string& componentID);														// Check if the Component is an Engine Component
		static void RegisterComponent(const std::string& componentID, CreatorFunc creator);									// Registers a Game-defined component type so it can be created from XML
	};
}