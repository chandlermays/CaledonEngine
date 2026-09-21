/*------------------------------
| File: GameObjectCreator.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "Utilities/CaledonParser.h"

#include <memory>
#include <string>
#include <vector>

namespace CE
{
	class ComponentFactory;																						// Forward Declaration of EngineComponentFactory
	class GameObject;																							// Forward Declaration of GameObject

	class GameObjectCreator
	{
	private:
		CaledonParser m_parser;																					// Caledon Parser

		ComponentFactory* m_pComponentFactory;																	// Engine Component Factory

		std::unique_ptr<GameObject> ParseGameObject(tinyxml2::XMLElement* pElement);							// Parse a GameObject from an XML Element (Root)

	public:
		GameObjectCreator();																					// Constructor
		~GameObjectCreator();																					// Destructor

		std::unique_ptr<GameObject> CreateGameObject(const std::string& fileData);								// Create a GameObject from an XML File
		std::vector<std::unique_ptr<GameObject>> CreateGameObjects(const std::string& fileData);				// Create multiple GameObjects from an XML File
	};
}