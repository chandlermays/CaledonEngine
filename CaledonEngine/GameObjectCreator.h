#pragma once
#include "CaledonEngine/Utilities/CaledonParser.h"

#include <vector>
#include <string>

namespace CE
{
	class ComponentFactory;																		// Forward Declaration of EngineComponentFactory
	class GameObject;																			// Forward Declaration of GameObject

	class GameObjectCreator
	{
	private:
		CaledonParser m_parser;																	// Caledon Parser

		ComponentFactory* m_pComponentFactory;													// Engine Component Factory

		GameObject* ParseGameObject(tinyxml2::XMLElement* pElement);							// Parse a GameObject from an XML Element (Root)

	public:
		GameObjectCreator();																	// Constructor
		~GameObjectCreator();																	// Destructor

		GameObject* CreateGameObject(const std::string& fileData);								// Create a GameObject from an XML File
		std::vector<GameObject*> CreateGameObjects(const std::string& fileData);				// Create multiple GameObjects from an XML File
	};
}