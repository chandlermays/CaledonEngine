#include "ComponentFactory.h"
#include "Core/GameObject.h"
#include "Utilities/ThirdParty/tinyxml2.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------
| --- CreateComponent: Creates a component of the given type and attaches it to the GameObject, using XML data if available --- |
-------------------------------------------------------------------------------------------------------------------------------*/
void CE::ComponentFactory::CreateComponent(GameObject* pGameObject, const std::string& componentID, tinyxml2::XMLElement* pElement)
{
	auto& registry = GetRegistry();
	auto it = registry.find(componentID);
	if (it == registry.end() || !it->second.xmlCreator)
		return;

	Component* pComponent = it->second.xmlCreator(pGameObject, pElement);
	if (pComponent)
	{
		pGameObject->AddComponent(pComponent);
	}
}

/*-----------------------------------------------------------------------
| --- RegisterComponent: Register a component type with the factory --- |
-----------------------------------------------------------------------*/
void CE::ComponentFactory::RegisterComponent(const std::string& typeName, const std::string& category,
	XmlCreatorFunc xmlCreator, DefaultCreatorFunc defaultCreator, std::vector<PropertyDescriptor> properties)
{
	RegistryEntry entry;
	entry.category = category;
	entry.xmlCreator = std::move(xmlCreator);
	entry.defaultCreator = std::move(defaultCreator);
	entry.properties = std::move(properties);
	GetRegistry()[typeName] = std::move(entry);
}

/*-------------------------------------------------------------------------------------------------------------------
| --- GetTypeInfo: Returns a pointer to the RegistryEntry for the given component type, or nullptr if not found --- |
-------------------------------------------------------------------------------------------------------------------*/
const CE::ComponentFactory::RegistryEntry* CE::ComponentFactory::GetTypeInfo(const std::string& typeName)
{
	auto& registry = GetRegistry();
	auto it = registry.find(typeName);
	return (it != registry.end()) ? &it->second : nullptr;
}

/*------------------------------------------------------------------------------------------------------------
| --- GetAllRegisteredTypes: Returns a const reference to the registry of all registered component types --- |
------------------------------------------------------------------------------------------------------------*/
const std::unordered_map<std::string, CE::ComponentFactory::RegistryEntry>& CE::ComponentFactory::GetAllRegisteredTypes()
{
	return GetRegistry();
}

/*--------------------------------------------------------------------------------------------------------
| --- IsEngineComponent: Returns true if the component is an engine component (category == "Engine") --- |
--------------------------------------------------------------------------------------------------------*/
bool CE::ComponentFactory::IsEngineComponent(const std::string& componentID)
{
	const RegistryEntry* pEntry = GetTypeInfo(componentID);
	return pEntry && pEntry->category == "Engine";
}



/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*--------------------------------------------------------------------------------------------
| --- GetRegistry: Returns a reference to the registry of all registered component types --- |
--------------------------------------------------------------------------------------------*/
std::unordered_map<std::string, CE::ComponentFactory::RegistryEntry>& CE::ComponentFactory::GetRegistry()
{
	static std::unordered_map<std::string, RegistryEntry> registry;
	return registry;
}