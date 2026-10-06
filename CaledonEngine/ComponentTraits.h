/*------------------------------
| File: ComponentTraits.h
| Author: Chandler Mays
------------------------------*/
#pragma once

#include "ComponentTypeInfo.h"
#include "Core/Component.h"
#include "Systems/Rendering/Components/SpriteComponent.h"
#include "Systems/Physics/Components/BoxCollider2D.h"
#include "Systems/Physics/Components/CircleCollider2D.h"
#include "Systems/Physics/Components/RigidBody2D.h"
#include "Core/Transform.h"
#include "Utilities/ThirdParty/tinyxml2.h"

#include <memory>
#include <string_view>
#include <vector>
#include <sstream>
#include <iomanip>

namespace CE
{
	template<typename T>
	struct ComponentTraits
	{
		static constexpr std::string_view StaticTypeName() { return "Unknown"; }
		static constexpr std::string_view StaticCategory() { return "Engine"; }
		static constexpr bool StaticAllowMultiple() { return true; }

		static std::unique_ptr<Component> CreateFromXML(GameObject* pGameObject, tinyxml2::XMLElement* pElement)
		{
			(void)pGameObject;
			(void)pElement;
			return std::make_unique<T>();
		}

		static std::unique_ptr<Component> CreateDefault()
		{
			return std::make_unique<T>();
		}

		static std::vector<PropertyDescriptor> GetProperties()
		{
			return {};
		}

		static tinyxml2::XMLElement* SerializeComponent(tinyxml2::XMLDocument* pDocument, const Component* pComponent)
		{
			(void)pDocument;
			(void)pComponent;
			return nullptr;
		}
	};

	// Explicit template specializations marked as inline
	template<>
	inline tinyxml2::XMLElement* ComponentTraits<SpriteComponent>::SerializeComponent(tinyxml2::XMLDocument* pDoc, const Component* pComponent)
	{
		const auto* pSprite = static_cast<const SpriteComponent*>(pComponent);
		auto* pElement = pDoc->NewElement("SpriteComponent");

		Color color = pSprite->GetColor();
		pElement->SetAttribute("r", color.r);
		pElement->SetAttribute("g", color.g);
		pElement->SetAttribute("b", color.b);
		pElement->SetAttribute("a", color.a);

		return pElement;
	}

	template<>
	inline tinyxml2::XMLElement* ComponentTraits<BoxCollider2D>::SerializeComponent(tinyxml2::XMLDocument* pDoc, const Component* pComponent)
	{
		const auto* pCollider = static_cast<const BoxCollider2D*>(pComponent);
		auto* pElement = pDoc->NewElement("BoxCollider2D");

		Vector2f size = pCollider->GetSize();
		std::ostringstream sizeStream;
		sizeStream << std::fixed << std::setprecision(6) << size.x << "," << size.y;
		pElement->SetAttribute("size", sizeStream.str().c_str());

		Vector2f offset = pCollider->GetOffset();
		std::ostringstream offsetStream;
		offsetStream << std::fixed << std::setprecision(6) << offset.x << "," << offset.y;
		pElement->SetAttribute("offset", offsetStream.str().c_str());

		pElement->SetAttribute("edgeRadius", pCollider->GetEdgeRadius());
		pElement->SetAttribute("isTrigger", pCollider->IsTrigger() ? "true" : "false");

		return pElement;
	}

	template<>
	inline tinyxml2::XMLElement* ComponentTraits<CircleCollider2D>::SerializeComponent(tinyxml2::XMLDocument* pDoc, const Component* pComponent)
	{
		const auto* pCollider = static_cast<const CircleCollider2D*>(pComponent);
		auto* pElement = pDoc->NewElement("CircleCollider2D");

		pElement->SetAttribute("radius", pCollider->GetRadius());

		Vector2f offset = pCollider->GetOffset();
		std::ostringstream offsetStream;
		offsetStream << std::fixed << std::setprecision(6) << offset.x << "," << offset.y;
		pElement->SetAttribute("offset", offsetStream.str().c_str());

		pElement->SetAttribute("isTrigger", pCollider->IsTrigger() ? "true" : "false");

		return pElement;
	}

	template<>
	inline tinyxml2::XMLElement* ComponentTraits<RigidBody2D>::SerializeComponent(tinyxml2::XMLDocument* pDoc, const Component* pComponent)
	{
		const auto* pBody = static_cast<const RigidBody2D*>(pComponent);
		auto* pElement = pDoc->NewElement("RigidBody2D");

		std::string bodyTypeStr;
		switch (pBody->GetBodyType())
		{
		case BodyType2D::Dynamic:   bodyTypeStr = "Dynamic"; break;
		case BodyType2D::Kinematic: bodyTypeStr = "Kinematic"; break;
		case BodyType2D::Static:    bodyTypeStr = "Static"; break;
		}
		pElement->SetAttribute("bodyType", bodyTypeStr.c_str());

		pElement->SetAttribute("mass", pBody->GetMass());
		pElement->SetAttribute("linearDamping", pBody->GetLinearDamping());
		pElement->SetAttribute("angularDamping", pBody->GetAngularDamping());
		pElement->SetAttribute("gravityScale", pBody->GetGravityScale());
		pElement->SetAttribute("simulated", pBody->IsSimulated() ? "true" : "false");
		pElement->SetAttribute("freezeRotation", pBody->GetFreezeRotation() ? "true" : "false");

		return pElement;
	}

	template<>
	inline tinyxml2::XMLElement* ComponentTraits<Transform>::SerializeComponent(tinyxml2::XMLDocument*, const Component*)
	{
		return nullptr;
	}
}