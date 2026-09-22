/*------------------------------
| File: ComponentFactory.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "ComponentTypeInfo.h"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

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
		using XmlCreatorFunc = std::function<std::unique_ptr<Component>(GameObject*, tinyxml2::XMLElement*)>;				// Constructs from XML data
		using DefaultCreatorFunc = std::function<std::unique_ptr<Component>()>;												// Constructs a blank instance — used by Editor's Add Component

		using RegisterFunc = std::function<void(const std::string&, const std::string&, XmlCreatorFunc,						// Registers a component type with the factory
			DefaultCreatorFunc, std::vector<PropertyDescriptor>, bool)>;

		struct RegistryEntry
		{
			std::string category;									// "Engine", or the active game's name — drives Add Component grouping
			XmlCreatorFunc xmlCreator;								// May be null if never XML-authored
			DefaultCreatorFunc defaultCreator;						// May be null if there's no sensible parameterless default
			std::vector<PropertyDescriptor> properties;				// What the Inspector can show/edit
			bool allowMultiple = true;								// Whether multiple instances of this component type can be added to a GameObject
		};

	private:
		static std::unordered_map<std::string, RegistryEntry>& GetRegistry();												// Returns a reference to the registry of all registered component types

	public:
		void CreateComponent(GameObject* pGameObject, const std::string& componentID, tinyxml2::XMLElement* pElement);		// Creates a component of the given type and attaches it to the GameObject, using XML data if available

		static void RegisterComponent(const std::string& typeName, const std::string& category,								// Register a component type with the factory
			XmlCreatorFunc xmlCreator, DefaultCreatorFunc defaultCreator = nullptr,
			std::vector<PropertyDescriptor> properties = {}, bool allowMultiple = true);

		static void UnregisterComponent(const std::string& typeName);														// Unregisters a component type from the factory

		static const RegistryEntry* GetTypeInfo(const std::string& typeName);												// Returns a pointer to the RegistryEntry for the given component type, or nullptr if not found
		static const std::unordered_map<std::string, RegistryEntry>& GetAllRegisteredTypes();								// Returns a const reference to the registry of all registered component types

		static bool IsEngineComponent(const std::string& componentID);														// Returns true if the component is an engine component (category == "Engine")
		static bool HasComponentOfType(const GameObject* pGameObject, const std::string& typeName);							// Returns true if the GameObject has a component of the given type

		static void Clear();																								// Clears all of the registry entries
	};
}