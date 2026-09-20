#include "GameObjectCreator.h"

#include "Core/GameObject.h"
#include "Core/Transform.h"
#include "Systems/Engine/LoggingManager.h"
#include "Utilities/ThirdParty/tinyxml2.h"
#include "ComponentFactory.h"

using namespace tinyxml2;

CE::GameObjectCreator::GameObjectCreator()
	: m_pComponentFactory{ std::make_unique<CE::ComponentFactory>() }
{}

CE::GameObjectCreator::~GameObjectCreator() = default;

std::unique_ptr<CE::GameObject> CE::GameObjectCreator::CreateGameObject(const std::string& fileData)
{
	if (!m_parser.Parse(fileData))
	{
		CE_LOG("Error parsing the XML data");
		return nullptr;
	}

	XMLElement* pRoot = m_parser.GetRootElement("GameObject");
	if (!pRoot)
	{
		CE_LOG("Error: Root element not found");
		return nullptr;
	}
	return ParseGameObject(pRoot);
}

std::vector<std::unique_ptr<CE::GameObject>> CE::GameObjectCreator::CreateGameObjects(const std::string& fileData)
{
	if (!m_parser.Parse(fileData))
	{
		CE_LOG("Error parsing the XML data");
		return {};
	}

	XMLElement* pRoot = m_parser.GetRootElement("GameObjects");
	if (!pRoot)
	{
		CE_LOG("Error: Root element not found");
		return {};
	}

	std::vector<std::unique_ptr<CE::GameObject>> gameObjects;
	for (XMLElement* pElement = pRoot->FirstChildElement("GameObject"); pElement;
		pElement = pElement->NextSiblingElement("GameObject"))
	{
		if (auto pGameObject = ParseGameObject(pElement))
			gameObjects.emplace_back(std::move(pGameObject));
	}
	return gameObjects;
}

std::unique_ptr<CE::GameObject> CE::GameObjectCreator::ParseGameObject(XMLElement* pElement)
{
	if (!pElement)
		return nullptr;

	auto pGameObject = std::make_unique<GameObject>();
	if (const char* pName = pElement->Attribute("name"))
		pGameObject->SetName(pName);
	if (const char* pTag = pElement->Attribute("tag"))
		pGameObject->SetTag(pTag);

	Vector2f position = Vector2f::Zero();
	Vector2f size = Vector2f::One();
	if (const char* pPosition = pElement->Attribute("position"))
		sscanf_s(pPosition, "%f,%f", &position.x, &position.y);
	if (const char* pSize = pElement->Attribute("size"))
		sscanf_s(pSize, "%f,%f", &size.x, &size.y);

	pGameObject->GetTransform().SetPosition(position);
	pGameObject->GetTransform().SetScale(size);

	for (XMLElement* pComponent = pElement->FirstChildElement(); pComponent;
		pComponent = pComponent->NextSiblingElement())
	{
		m_pComponentFactory->CreateComponent(pGameObject.get(), pComponent->Name(), pComponent);
	}

	for (XMLElement* pChildElement = pElement->FirstChildElement("GameObject"); pChildElement;
		pChildElement = pChildElement->NextSiblingElement("GameObject"))
	{
		if (auto pChild = ParseGameObject(pChildElement))
			pGameObject->AddChild(std::move(pChild));
	}

	pGameObject->Initialize();
	return pGameObject;
}
