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
	class GameObject;																							// Forward Declaration of GameObject

	class GameObjectCreator
	{
	private:
		CaledonParser m_parser;																					// Caledon Parser

		std::unique_ptr<GameObject> ParseGameObject(tinyxml2::XMLElement* pElement);							// Parse a GameObject from an XML Element (Root)

	public:
		GameObjectCreator() = default;																			// Constructor
		~GameObjectCreator() = default;																			// Destructor
		GameObjectCreator(const GameObjectCreator&) = delete;													// Prevent copy-construction
		GameObjectCreator& operator=(const GameObjectCreator&) = delete;										// Prevent copy-assignment
		GameObjectCreator(GameObjectCreator&&) = delete;														// Prevent move-construction
		GameObjectCreator& operator=(GameObjectCreator&&) = delete;												// Prevent move-assignment

		std::unique_ptr<GameObject> CreateGameObject(const std::string& fileData);								// Create a GameObject from an XML File
		std::vector<std::unique_ptr<GameObject>> CreateGameObjects(const std::string& fileData);				// Create multiple GameObjects from an XML File
	};
}