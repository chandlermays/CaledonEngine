#include "GameObjectCreator.h"
#include "CaledonEngine/Utilities/ThirdParty/tinyxml2.h"
#include "ComponentFactory.h"
#include "CaledonEngine/Core/GameObject.h"
#include "CaledonEngine/Systems/Engine/LoggingManager.h"

#include <string>

using namespace tinyxml2;

///////////////////////////////////////
/*-------------------------------------
| --- Public Function Definitions --- |
-------------------------------------*/
///////////////////////////////////////

/*---------------------
| --- Constructor --- |
---------------------*/

CE::GameObjectCreator::GameObjectCreator()
	: m_pComponentFactory{ new CE::ComponentFactory() }
{
}

/*--------------------
| --- Destructor --- |
--------------------*/

CE::GameObjectCreator::~GameObjectCreator()
{
	delete m_pComponentFactory;
	m_pComponentFactory = nullptr;
}


/*----------------------------------------------------------------
| --- CreateGameObject: Create a GameObject from an XML File --- |
----------------------------------------------------------------*/

CE::GameObject* CE::GameObjectCreator::CreateGameObject(const std::string& fileData)
{
	if (!m_parser.Parse(fileData))
	{
		CE_LOG("Error parsing the XML data");
		return nullptr;
	}

	XMLElement* pRoot = m_parser.GetRootElement("GameObject");
	if (pRoot == nullptr)
	{
		CE_LOG("Error: Root element not found");
		return nullptr;
	}

	return ParseGameObject(pRoot);
}

/*-------------------------------------------------------------------------
| --- CreateGameObjects: Create multiple GameObjects from an XML File --- |
-------------------------------------------------------------------------*/

std::vector<CE::GameObject*> CE::GameObjectCreator::CreateGameObjects(const std::string& fileData)
{
	if (!m_parser.Parse(fileData))
	{
		CE_LOG("Error parsing the XML data");
		return {};
	}

	XMLElement* pRoot = m_parser.GetRootElement("GameObjects");
	if (pRoot == nullptr)
	{
		CE_LOG("Error: Root element not found");
		return {};
	}

	std::vector<CE::GameObject*> gameObjects;
	for (XMLElement* pElement = pRoot->FirstChildElement("GameObject"); pElement != nullptr; pElement = pElement->NextSiblingElement("GameObject"))
	{
		CE::GameObject* pGameObject = ParseGameObject(pElement);
		if (pGameObject)
		{
			gameObjects.emplace_back(pGameObject);
		}
	}

	return gameObjects;
}



////////////////////////////////////////
/*--------------------------------------
| --- Private Function Definitions --- |
--------------------------------------*/
////////////////////////////////////////

/*--------------------------------------------------------------------------
| --- CreateGameObject: Create a GameObject from an XML Element (Root) --- |
--------------------------------------------------------------------------*/

CE::GameObject* CE::GameObjectCreator::ParseGameObject(XMLElement* pElement)
{
	// Create the parent GameObject
	CE::GameObject* pGameObject = new GameObject();

	// Parse the Name of the GameObject
	const char* pName = pElement->Attribute("name");
	if (pName)
	{
		pGameObject->SetName(pName);
	}

	// Parse the Tag of the GameObject
	const char* pTag = pElement->Attribute("tag");
	if (pTag)
	{
		pGameObject->SetTag(pTag);
	}

	// Parse and Set the Position and Size of the GameObject
	Vector2f position, size;
	sscanf_s(pElement->Attribute("position"), "%f,%f", &position.x, &position.y);
	sscanf_s(pElement->Attribute("size"), "%f,%f", &size.x, &size.y);

	pGameObject->GetTransform().SetPosition(position);
	pGameObject->GetTransform().SetScale(size);

	// Attach components to the parent GameObject
	for (XMLElement* pComponent = pElement->FirstChildElement(); pComponent != nullptr; pComponent = pComponent->NextSiblingElement())
	{
		std::string componentID = pComponent->Name();

		m_pComponentFactory->CreateComponent(pGameObject, componentID, pComponent);
	}

	// Create and add child GameObjects
	for (XMLElement* pChildElement = pElement->FirstChildElement("GameObject"); pChildElement != nullptr; pChildElement = pChildElement->NextSiblingElement("GameObject"))
	{
		CE::GameObject* pChildGameObject = ParseGameObject(pChildElement);
		if (pChildGameObject)
		{
			pGameObject->AddChild(pChildGameObject);
		}
	}

	// Call Initialize() after all components and children are attached
	pGameObject->Initialize();

	return pGameObject;
}