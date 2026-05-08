#pragma once

#include <string>

namespace tinyxml2
{
	class XMLElement;
}

namespace CE
{
	class GameObject;

	class ComponentFactory
	{
	private:
		void AddSpriteComponent(GameObject* pGameObject, [[maybe_unused]] tinyxml2::XMLElement* pElement);					// Attach a Sprite Component to the GameObject

	public:
		void CreateComponent(GameObject* pGameObject, const std::string& componentID, tinyxml2::XMLElement* pElement);		// Create a Component for the GameObject
		static bool IsEngineComponent(const std::string& componentID);														// Check if the Component is an Engine Component
	};
}