#pragma once
#include "ComponentTypeInfo.h"

#include <string>
#include <unordered_map>
#include <functional>

namespace tinyxml2 { class XMLElement; }

namespace CE
{
	class GameObject;
	class Component;

	class ComponentFactory
	{
	public:
		using XmlCreatorFunc = std::function<Component* (GameObject*, tinyxml2::XMLElement*)>;				// Constructs from XML data
		using DefaultCreatorFunc = std::function<Component* ()>;											// Constructs a blank instance — used by Editor's Add Component

		struct RegistryEntry
		{
			std::string category;									// "Engine", or the active game's name — drives Add Component grouping
			XmlCreatorFunc xmlCreator;								// May be null if never XML-authored
			DefaultCreatorFunc defaultCreator;						// May be null if there's no sensible parameterless default
			std::vector<PropertyDescriptor> properties;				// What the Inspector can show/edit
		};

	private:
		static std::unordered_map<std::string, RegistryEntry>& GetRegistry();												// Returns a reference to the registry of all registered component types

	public:
		void CreateComponent(GameObject* pGameObject, const std::string& componentID, tinyxml2::XMLElement* pElement);		// Creates a component of the given type and attaches it to the GameObject, using XML data if available

		static void RegisterComponent(const std::string& typeName, const std::string& category,								// Register a component type with the factory
			XmlCreatorFunc xmlCreator, DefaultCreatorFunc defaultCreator = nullptr,
			std::vector<PropertyDescriptor> properties = {});

		static const RegistryEntry* GetTypeInfo(const std::string& typeName);												// Returns a pointer to the RegistryEntry for the given component type, or nullptr if not found
		static const std::unordered_map<std::string, RegistryEntry>& GetAllRegisteredTypes();								// Returns a const reference to the registry of all registered component types

		static bool IsEngineComponent(const std::string& componentID);														// Returns true if the component is an engine component (category == "Engine")
	};
}