#pragma once
#include "ComponentTypeInfo.h"
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace tinyxml2 { class XMLElement; }

namespace CE
{
	class GameObject;
	class Component;

	class ComponentFactory
	{
	public:
		// Raw pointers are retained at the DLL ABI boundary. CreateComponent and
		// the editor acquire ownership immediately with std::unique_ptr.
		using XmlCreatorFunc = std::function<Component*(GameObject*, tinyxml2::XMLElement*)>;
		using DefaultCreatorFunc = std::function<Component*()>;
		using RegisterFunc = std::function<void(const std::string&, const std::string&, XmlCreatorFunc,
			DefaultCreatorFunc, std::vector<PropertyDescriptor>, bool)>;

		struct RegistryEntry
		{
			std::string category;
			XmlCreatorFunc xmlCreator;
			DefaultCreatorFunc defaultCreator;
			std::vector<PropertyDescriptor> properties;
			bool allowMultiple = true;
		};

	private:
		static std::unordered_map<std::string, RegistryEntry>& GetRegistry();

	public:
		void CreateComponent(GameObject*, const std::string&, tinyxml2::XMLElement*);
		static void RegisterComponent(const std::string&, const std::string&, XmlCreatorFunc,
			DefaultCreatorFunc = nullptr, std::vector<PropertyDescriptor> = {}, bool = true);
		static void UnregisterComponent(const std::string&);
		static const RegistryEntry* GetTypeInfo(const std::string&);
		static const std::unordered_map<std::string, RegistryEntry>& GetAllRegisteredTypes();
		static bool IsEngineComponent(const std::string&);
		static bool HasComponentOfType(const GameObject*, const std::string&);
	};
}
