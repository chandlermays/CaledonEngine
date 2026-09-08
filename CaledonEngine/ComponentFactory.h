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
		using XmlCreatorFunc = std::function<Component* (GameObject*, tinyxml2::XMLElement*)>;		// Constructs from XML data
		using DefaultCreatorFunc = std::function<Component* ()>;										// Constructs a blank instance — used by Editor's Add Component

		struct RegistryEntry
		{
			std::string category;								// "Engine", or the active game's name — drives Add Component grouping
			XmlCreatorFunc xmlCreator;							// May be null if never XML-authored
			DefaultCreatorFunc defaultCreator;					// May be null if there's no sensible parameterless default
			std::vector<PropertyDescriptor> properties;		// What the Inspector can show/edit
		};

	private:
		static std::unordered_map<std::string, RegistryEntry>& GetRegistry();

	public:
		void CreateComponent(GameObject* pGameObject, const std::string& componentID, tinyxml2::XMLElement* pElement);

		static void RegisterComponent(const std::string& typeName, const std::string& category,
			XmlCreatorFunc xmlCreator, DefaultCreatorFunc defaultCreator = nullptr,
			std::vector<PropertyDescriptor> properties = {});

		static const RegistryEntry* GetTypeInfo(const std::string& typeName);						// Used by the Inspector
		static const std::unordered_map<std::string, RegistryEntry>& GetAllRegisteredTypes();			// Used to build the Add Component menu

		static bool IsEngineComponent(const std::string& componentID);
	};
}