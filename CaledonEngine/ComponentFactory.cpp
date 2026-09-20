#include "ComponentFactory.h"
#include "Core/GameObject.h"
#include "Core/Component.h"
#include "Systems/Engine/LoggingManager.h"
#include "Utilities/ThirdParty/tinyxml2.h"

void CE::ComponentFactory::CreateComponent(GameObject* pGameObject, const std::string& componentID, tinyxml2::XMLElement* pElement)
{
	auto& registry = GetRegistry();
	auto it = registry.find(componentID);
	if (!pGameObject || it == registry.end() || !it->second.xmlCreator)
		return;

	if (!it->second.allowMultiple && HasComponentOfType(pGameObject, componentID))
	{
		CE_LOG("ComponentFactory::CreateComponent - '{}' already exists on '{}' and does not allow multiple instances; skipping.", componentID, pGameObject->GetName());
		return;
	}

	// Creator callbacks remain raw-pointer based for the game-module ABI. Ownership
	// is acquired immediately at this engine boundary.
	if (Component* pComponent = it->second.xmlCreator(pGameObject, pElement))
		pGameObject->AddComponent(std::unique_ptr<Component>(pComponent));
}

void CE::ComponentFactory::RegisterComponent(const std::string& typeName, const std::string& category,
	XmlCreatorFunc xmlCreator, DefaultCreatorFunc defaultCreator, std::vector<PropertyDescriptor> properties, bool allowMultiple)
{
	RegistryEntry entry;
	entry.category = category;
	entry.xmlCreator = std::move(xmlCreator);
	entry.defaultCreator = std::move(defaultCreator);
	entry.properties = std::move(properties);
	entry.allowMultiple = allowMultiple;
	GetRegistry()[typeName] = std::move(entry);
}

void CE::ComponentFactory::UnregisterComponent(const std::string& typeName) { GetRegistry().erase(typeName); }

const CE::ComponentFactory::RegistryEntry* CE::ComponentFactory::GetTypeInfo(const std::string& typeName)
{
	auto& registry = GetRegistry();
	auto it = registry.find(typeName);
	return it != registry.end() ? &it->second : nullptr;
}

const std::unordered_map<std::string, CE::ComponentFactory::RegistryEntry>& CE::ComponentFactory::GetAllRegisteredTypes()
{
	return GetRegistry();
}

bool CE::ComponentFactory::IsEngineComponent(const std::string& componentID)
{
	const RegistryEntry* pEntry = GetTypeInfo(componentID);
	return pEntry && pEntry->category == "Engine";
}

bool CE::ComponentFactory::HasComponentOfType(const GameObject* pGameObject, const std::string& typeName)
{
	if (!pGameObject)
		return false;
	for (const auto& pComponent : pGameObject->GetAllComponents())
		if (pComponent && pComponent->GetTypeName() == typeName)
			return true;
	return false;
}
