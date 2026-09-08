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
#include "Utilities/ThirdParty/tinyxml2.h"

using namespace tinyxml2;

CE::Component* CE::BuiltInComponents::CreateSpriteComponentFromXml(GameObject*, tinyxml2::XMLElement* pElement)
{
	const char* pSpritesheet = pElement->Attribute("spritesheet");
	int frameWidth = pElement->IntAttribute("frameWidth");
	int frameHeight = pElement->IntAttribute("frameHeight");
	float scale = pElement->FloatAttribute("scale");
	const char* pShapeType = pElement->Attribute("shape");

	SpriteComponent* pSpriteCmp = nullptr;

	if (pSpritesheet && frameWidth > 0 && frameHeight > 0)
	{
		pSpriteCmp = new SpriteComponent(pSpritesheet, frameWidth, frameHeight, scale);

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
		pSpriteCmp = new SpriteComponent();

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
		pSpriteCmp = new SpriteComponent();
	}

	return pSpriteCmp;
}

CE::Component* CE::BuiltInComponents::CreateBoxCollider2DFromXml(GameObject*, tinyxml2::XMLElement* pElement)
{
	BoxCollider2D* pCollider = new BoxCollider2D();

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

void CE::BuiltInComponents::RegisterAll()
{
	ComponentFactory::RegisterComponent("SpriteComponent", "Engine",
		CreateSpriteComponentFromXml,
		[]() -> Component* { return new SpriteComponent(); },
		{
			MakeProperty<SpriteComponent>("Color",
				[](const SpriteComponent* p) -> PropertyValue { return p->GetColor(); },
				[](SpriteComponent* p, const PropertyValue& v) { p->SetColor(std::get<Color>(v)); })
		});

		ComponentFactory::RegisterComponent("BoxCollider2D", "Engine",
			CreateBoxCollider2DFromXml,
			[]() -> Component* { return new BoxCollider2D(); },
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

		// Transform is never looked up via CreateComponent — GameObject's own constructor always
		// builds one directly — but registering it anyway lets the Inspector show its properties
		// through the same generic path as everything else, rather than special-casing it.
		ComponentFactory::RegisterComponent("Transform", "Engine",
			nullptr,		// never XML-dispatched
			nullptr,		// never "Add Component"-able — every GameObject already has exactly one
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