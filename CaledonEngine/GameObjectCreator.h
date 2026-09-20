#pragma once
#include "Utilities/CaledonParser.h"

#include <memory>
#include <string>
#include <vector>

namespace CE
{
	class ComponentFactory;
	class GameObject;

	class GameObjectCreator
	{
	private:
		CaledonParser m_parser;
		std::unique_ptr<ComponentFactory> m_pComponentFactory;

		std::unique_ptr<GameObject> ParseGameObject(tinyxml2::XMLElement* pElement);

	public:
		GameObjectCreator();
		~GameObjectCreator();
		GameObjectCreator(const GameObjectCreator&) = delete;
		GameObjectCreator& operator=(const GameObjectCreator&) = delete;

		std::unique_ptr<GameObject> CreateGameObject(const std::string& fileData);
		std::vector<std::unique_ptr<GameObject>> CreateGameObjects(const std::string& fileData);
	};
}
