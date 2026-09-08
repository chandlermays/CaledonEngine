#pragma once
#include "Systems/Rendering/Color.h"
#include "Utilities/Math/Vector2.h"

#include <string>
#include <vector>
#include <functional>
#include <variant>

namespace CE
{
	class Component;

	// Extend this list only when a real property actually needs a new type — the
	// getter/setter machinery below doesn't care how many alternatives exist.
	using PropertyValue = std::variant<int, float, bool, std::string, Vector2f, Color>;

	struct PropertyDescriptor
	{
		std::string name;												// Label shown in the Inspector
		std::function<PropertyValue(const Component*)> getter;
		std::function<void(Component*, const PropertyValue&)> setter;
	};

	// Wraps two lambdas typed against the concrete component so registration code
	// never hand-writes the Component* cast itself.
	template<typename ComponentT>
	PropertyDescriptor MakeProperty(
		const std::string& name,
		std::function<PropertyValue(const ComponentT*)> getter,
		std::function<void(ComponentT*, const PropertyValue&)> setter)
	{
		PropertyDescriptor descriptor;
		descriptor.name = name;
		descriptor.getter = [getter](const Component* pComponent) -> PropertyValue
			{
				return getter(static_cast<const ComponentT*>(pComponent));
			};
		descriptor.setter = [setter](Component* pComponent, const PropertyValue& value)
			{
				setter(static_cast<ComponentT*>(pComponent), value);
			};
		return descriptor;
	}
}