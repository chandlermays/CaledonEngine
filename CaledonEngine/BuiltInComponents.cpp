/*------------------------------
| File: BuiltInComponents.h
| Author: Chandler Mays
------------------------------*/
#include "BuiltInComponents.h"

#include "ComponentFactory.h"
#include "Core/GameObject.h"
#include "Core/Transform.h"
#include "Systems/Rendering/Components/SpriteComponent.h"
#include "Systems/Rendering/Shapes/Square.h"
#include "Systems/Rendering/Shapes/Circle.h"
#include "Systems/Rendering/Shapes/Triangle.h"
#include "Systems/Rendering/Shapes/Capsule.h"
#include "Systems/Rendering/Sprite.h"
#include "Systems/Physics/Components/BoxCollider2D.h"
#include "Systems/Physics/Components/CircleCollider2D.h"
#include "Systems/Physics/Components/RigidBody2D.h"
#include "Utilities/ThirdParty/tinyxml2.h"

#include <algorithm>
#include <string>

using namespace tinyxml2;

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*----------------------------------------------------------------------------------
| --- RegisterAll: Registers all built-in components with the ComponentFactory --- |
----------------------------------------------------------------------------------*/
void CE::BuiltInComponents::RegisterAll()
{
	// TODO: Register other built-in components here as needed

	// Components are allowed to have multiple instances on the same GameObject by default.
	// Explicitly set flag to 'false' for components that should not allow multiple instances.

	ComponentFactory::RegisterComponent("SpriteComponent", "Engine",
		CreateSpriteComponentFromXml,
		[]() -> std::unique_ptr<Component>
		{
			auto pSprite = std::make_unique<SpriteComponent>();
			pSprite->SetSprite(Sprite::CreateFromShape(std::make_unique<Square>(Color::White(), 50)));
			return pSprite;
		},
	{
		MakeProperty<SpriteComponent>("Color",
			[](const SpriteComponent* p) -> PropertyValue { return p->GetColor(); },
			[](SpriteComponent* p, const PropertyValue& v) { p->SetColor(std::get<Color>(v)); })
	},
		false);	// SpriteComponent is not allowed to have multiple instances on the same GameObject

	ComponentFactory::RegisterComponent("BoxCollider2D", "Engine",
		CreateBoxCollider2DFromXml,
		[]() -> std::unique_ptr<Component> { return std::make_unique<BoxCollider2D>(); },
		{
			MakeProperty<BoxCollider2D>("Size",
				[](const BoxCollider2D* p) -> PropertyValue { return p->GetSize(); },
				[](BoxCollider2D* p, const PropertyValue& v) { p->SetSize(std::get<Vector2f>(v)); }),
			MakeProperty<BoxCollider2D>("Offset",
				[](const BoxCollider2D* p) -> PropertyValue { return p->GetOffset(); },
				[](BoxCollider2D* p, const PropertyValue& v) { p->SetOffset(std::get<Vector2f>(v)); }),
			MakeProperty<BoxCollider2D>("Edge Radius",
				[](const BoxCollider2D* p) -> PropertyValue { return p->GetEdgeRadius(); },
				[](BoxCollider2D* p, const PropertyValue& v) { p->SetEdgeRadius(std::get<float>(v)); }),
			MakeProperty<BoxCollider2D>("Is Trigger",
				[](const BoxCollider2D* p) -> PropertyValue { return p->IsTrigger(); },
				[](BoxCollider2D* p, const PropertyValue& v) { p->SetTrigger(std::get<bool>(v)); })
		});

		ComponentFactory::RegisterComponent("CircleCollider2D", "Engine",
			CreateCircleCollider2DFromXml,
			[]() -> std::unique_ptr<Component> { return std::make_unique<CircleCollider2D>(); },
		{
			MakeProperty<CircleCollider2D>("Radius",
				[](const CircleCollider2D* p) -> PropertyValue { return p->GetRadius(); },
				[](CircleCollider2D* p, const PropertyValue& v) { p->SetRadius(std::get<float>(v)); }),
			MakeProperty<CircleCollider2D>("Offset",
				[](const CircleCollider2D* p) -> PropertyValue { return p->GetOffset(); },
				[](CircleCollider2D* p, const PropertyValue& v) { p->SetOffset(std::get<Vector2f>(v)); }),
			MakeProperty<CircleCollider2D>("Is Trigger",
				[](const CircleCollider2D* p) -> PropertyValue { return p->IsTrigger(); },
				[](CircleCollider2D* p, const PropertyValue& v) { p->SetTrigger(std::get<bool>(v)); })
		});

		ComponentFactory::RegisterComponent("RigidBody2D", "Engine",
			CreateRigidBody2DFromXml,
			[]() -> std::unique_ptr<Component> { return std::make_unique<RigidBody2D>(); },
		{
			MakeProperty<RigidBody2D>("Body Type",		// 0 = Dynamic, 1 = Kinematic, 2 = Static
				[](const RigidBody2D* p) -> PropertyValue { return static_cast<int>(p->GetBodyType()); },
				[](RigidBody2D* p, const PropertyValue& v) { p->SetBodyType(static_cast<BodyType2D>(std::clamp(std::get<int>(v), 0, 2))); }),
			MakeProperty<RigidBody2D>("Mass",
				[](const RigidBody2D* p) -> PropertyValue { return p->GetMass(); },
				[](RigidBody2D* p, const PropertyValue& v) { p->SetMass(std::get<float>(v)); }),
			MakeProperty<RigidBody2D>("Linear Damping",
				[](const RigidBody2D* p) -> PropertyValue { return p->GetLinearDamping(); },
				[](RigidBody2D* p, const PropertyValue& v) { p->SetLinearDamping(std::get<float>(v)); }),
			MakeProperty<RigidBody2D>("Angular Damping",
				[](const RigidBody2D* p) -> PropertyValue { return p->GetAngularDamping(); },
				[](RigidBody2D* p, const PropertyValue& v) { p->SetAngularDamping(std::get<float>(v)); }),
			MakeProperty<RigidBody2D>("Gravity Scale",
				[](const RigidBody2D* p) -> PropertyValue { return p->GetGravityScale(); },
				[](RigidBody2D* p, const PropertyValue& v) { p->SetGravityScale(std::get<float>(v)); }),
			MakeProperty<RigidBody2D>("Simulated",
				[](const RigidBody2D* p) -> PropertyValue { return p->IsSimulated(); },
				[](RigidBody2D* p, const PropertyValue& v) { p->SetSimulated(std::get<bool>(v)); }),
			MakeProperty<RigidBody2D>("Freeze Rotation",
				[](const RigidBody2D* p) -> PropertyValue { return p->GetFreezeRotation(); },
				[](RigidBody2D* p, const PropertyValue& v) { p->SetFreezeRotation(std::get<bool>(v)); })
		},
			false);	// Only one RigidBody2D per GameObject

		//---------------------------------------------------------------------------------------
		// Transform is never looked up via CreateComponent — GameObject's own constructor always
		// builds one directly — but registering it anyway lets the Inspector show its properties
		// through the same generic path as everything else, rather than special-casing it.
		ComponentFactory::RegisterComponent("Transform", "Engine",
			nullptr,
			nullptr,
			{
				MakeProperty<Transform>("Position",
					[](const Transform* p) -> PropertyValue { return p->GetPosition(); },
					[](Transform* p, const PropertyValue& v) { p->SetPosition(std::get<Vector2f>(v)); }),
				MakeProperty<Transform>("Rotation",
					[](const Transform* p) -> PropertyValue { return p->GetRotation(); },
					[](Transform* p, const PropertyValue& v) { p->SetRotation(std::get<float>(v)); }),
				MakeProperty<Transform>("Scale",
					[](const Transform* p) -> PropertyValue { return p->GetScale(); },
					[](Transform* p, const PropertyValue& v) { p->SetScale(std::get<Vector2f>(v)); })
			});
}



/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*-------------------------------------------------------------------------------------
| --- CreateSpriteComponentFromXml: Creates a SpriteComponent from an XML element --- |
-------------------------------------------------------------------------------------*/
std::unique_ptr<CE::Component> CE::BuiltInComponents::CreateSpriteComponentFromXml(GameObject*, tinyxml2::XMLElement* pElement)
{
	const char* pSpritesheet = pElement->Attribute("spritesheet");
	int frameWidth = pElement->IntAttribute("frameWidth");
	int frameHeight = pElement->IntAttribute("frameHeight");
	float scale = pElement->FloatAttribute("scale");
	const char* pShapeType = pElement->Attribute("shape");

	std::unique_ptr<SpriteComponent> pSpriteCmp;

	if (pSpritesheet && frameWidth > 0 && frameHeight > 0)
	{
		pSpriteCmp = std::make_unique<SpriteComponent>(pSpritesheet, frameWidth, frameHeight, scale);

		XMLElement* pColor = pElement->FirstChildElement("Color");
		if (pColor != nullptr)
		{
			unsigned char r = static_cast<unsigned char>(pColor->UnsignedAttribute("r"));
			unsigned char g = static_cast<unsigned char>(pColor->UnsignedAttribute("g"));
			unsigned char b = static_cast<unsigned char>(pColor->UnsignedAttribute("b"));
			unsigned char a = static_cast<unsigned char>(pColor->UnsignedAttribute("a"));
			pSpriteCmp->SetColor(r, g, b, a);
		}
	}
	else if (pShapeType)
	{
		pSpriteCmp = std::make_unique<SpriteComponent>();

		Color shapeColor = Color::White();
		XMLElement* pColor = pElement->FirstChildElement("Color");
		if (pColor != nullptr)
		{
			shapeColor.r = static_cast<unsigned char>(pColor->UnsignedAttribute("r"));
			shapeColor.g = static_cast<unsigned char>(pColor->UnsignedAttribute("g"));
			shapeColor.b = static_cast<unsigned char>(pColor->UnsignedAttribute("b"));
			shapeColor.a = static_cast<unsigned char>(pColor->UnsignedAttribute("a"));
		}

		bool isFilled = pElement->BoolAttribute("filled", true);
		int width = pElement->IntAttribute("width", 100);
		int height = pElement->IntAttribute("height", 100);

		std::string shapeType = pShapeType;
		std::unique_ptr<Shape> pShape;

		if (shapeType == "Square")
		{
			pShape = std::make_unique<Square>(shapeColor, width, isFilled);
		}
		else if (shapeType == "Circle")
		{
			pShape = std::make_unique<Circle>(shapeColor, pElement->IntAttribute("radius", width / 2), isFilled);
		}
		else if (shapeType == "Triangle")
		{
			pShape = std::make_unique<Triangle>(shapeColor, width, height, isFilled);
		}
		else if (shapeType == "Capsule")
		{
			pShape = std::make_unique<Capsule>(shapeColor, width, height, isFilled);
		}

		if (pShape)
		{
			pSpriteCmp->SetSprite(Sprite::CreateFromShape(std::move(pShape)));
		}
	}
	else
	{
		pSpriteCmp = std::make_unique<SpriteComponent>();
	}

	return pSpriteCmp;
}

/*---------------------------------------------------------------------------------
| --- CreateBoxCollider2DFromXml: Creates a BoxCollider2D from an XML element --- |
---------------------------------------------------------------------------------*/
std::unique_ptr<CE::Component> CE::BuiltInComponents::CreateBoxCollider2DFromXml(GameObject*, tinyxml2::XMLElement* pElement)
{
	auto pCollider = std::make_unique<BoxCollider2D>();

	const char* pSize = pElement->Attribute("size");
	if (pSize)
	{
		Vector2f size = Vector2f::One();
		sscanf_s(pSize, "%f,%f", &size.x, &size.y);
		pCollider->SetSize(size);
	}

	const char* pOffset = pElement->Attribute("offset");
	if (pOffset)
	{
		Vector2f offset = Vector2f::Zero();
		sscanf_s(pOffset, "%f,%f", &offset.x, &offset.y);
		pCollider->SetOffset(offset);
	}

	pCollider->SetEdgeRadius(pElement->FloatAttribute("edgeRadius", 0.0f));
	pCollider->SetTrigger(pElement->BoolAttribute("isTrigger", false));

	return pCollider;
}

/*---------------------------------------------------------------------------------------
| --- CreateCircleCollider2DFromXml: Creates a CircleCollider2D from an XML element --- |
---------------------------------------------------------------------------------------*/
std::unique_ptr<CE::Component> CE::BuiltInComponents::CreateCircleCollider2DFromXml(GameObject*, tinyxml2::XMLElement* pElement)
{
	auto pCollider = std::make_unique<CircleCollider2D>();

	pCollider->SetRadius(pElement->FloatAttribute("radius", pCollider->GetRadius()));

	const char* pOffset = pElement->Attribute("offset");
	if (pOffset)
	{
		Vector2f offset = Vector2f::Zero();
		sscanf_s(pOffset, "%f,%f", &offset.x, &offset.y);
		pCollider->SetOffset(offset);
	}

	pCollider->SetTrigger(pElement->BoolAttribute("isTrigger", false));

	return pCollider;
}

/*-----------------------------------------------------------------------------
| --- CreateRigidBody2DFromXml: Creates a RigidBody2D from an XML element --- |
-----------------------------------------------------------------------------*/
std::unique_ptr<CE::Component> CE::BuiltInComponents::CreateRigidBody2DFromXml(GameObject*, tinyxml2::XMLElement* pElement)
{
	auto pBody = std::make_unique<RigidBody2D>();

	const char* pBodyType = pElement->Attribute("bodyType");
	if (pBodyType)
	{
		std::string bodyType = pBodyType;

		if (bodyType == "Kinematic")
		{
			pBody->SetBodyType(BodyType2D::Kinematic);
		}
		else if (bodyType == "Static")
		{
			pBody->SetBodyType(BodyType2D::Static);
		}
		else
		{
			pBody->SetBodyType(BodyType2D::Dynamic);
		}
	}

	// Attributes that aren't specified keep the component's own defaults
	pBody->SetMass(pElement->FloatAttribute("mass", pBody->GetMass()));
	pBody->SetLinearDamping(pElement->FloatAttribute("linearDamping", pBody->GetLinearDamping()));
	pBody->SetAngularDamping(pElement->FloatAttribute("angularDamping", pBody->GetAngularDamping()));
	pBody->SetGravityScale(pElement->FloatAttribute("gravityScale", pBody->GetGravityScale()));
	pBody->SetSimulated(pElement->BoolAttribute("simulated", pBody->IsSimulated()));
	pBody->SetFreezeRotation(pElement->BoolAttribute("freezeRotation", pBody->GetFreezeRotation()));

	return pBody;
}