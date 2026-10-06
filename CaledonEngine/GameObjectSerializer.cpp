/*------------------------------
| File: GameObjectSerializer.cpp
| Author: Chandler Mays
------------------------------*/
#include "GameObjectSerializer.h"

#include "Core/Scene.h"
#include "Core/GameObject.h"
#include "Core/Transform.h"
#include "Core/Component.h"
#include "ComponentFactory.h"
#include "ComponentTraits.h"
#include "Utilities/Math/Vector2.h"
#include "Systems/Engine/LoggingManager.h"
#include "Utilities/ThirdParty/tinyxml2.h"
#include "Systems/Physics/Components/BoxCollider2D.h"
#include "Systems/Physics/Components/CircleCollider2D.h"
#include "Systems/Physics/Components/RigidBody2D.h"
#include "Systems/Rendering/Components/SpriteComponent.h"

#include <sstream>
#include <iomanip>

using namespace tinyxml2;

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*------------------------------------------------------------------
| --- SerializeScene: Convert an entire scene to an XML string --- |
------------------------------------------------------------------*/
std::string CE::GameObjectSerializer::SerializeScene(const CE::Scene& scene)
{
	XMLDocument doc;
	XMLDeclaration* pDecl = doc.NewDeclaration();
	doc.InsertFirstChild(pDecl);

	XMLElement* pRoot = doc.NewElement("GameObjects");
	doc.InsertEndChild(pRoot);

	// Serialize each root-level GameObject in the scene
	const auto& gameObjects = scene.GetGameObjects();
	for (const auto& pGameObject : gameObjects)
	{
		if (!pGameObject)
			continue;

		XMLElement* pGameObjectElement = SerializeGameObjectElement(&doc, pGameObject.get());
		if (pGameObjectElement)
		{
			pRoot->InsertEndChild(pGameObjectElement);
		}
	}

	XMLPrinter printer;
	doc.Print(&printer);
	return printer.CStr();
}

/*------------------------------------------------------------------------
| --- SerializeGameObject: Convert a single GameObject to XML string --- |
------------------------------------------------------------------------*/
std::string CE::GameObjectSerializer::SerializeGameObject(const CE::Scene&, const GameObject* pGameObject)
{
	if (!pGameObject)
		return "";

	XMLDocument doc;
	XMLDeclaration* pDecl = doc.NewDeclaration();
	doc.InsertFirstChild(pDecl);

	XMLElement* pGameObjectElement = SerializeGameObjectElement(&doc, pGameObject);
	if (pGameObjectElement)
	{
		doc.InsertEndChild(pGameObjectElement);
	}

	XMLPrinter printer;
	doc.Print(&printer);
	return printer.CStr();
}



/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*-----------------------------------------------------------------------------------------------
| --- SerializeGameObjectElement: 
-----------------------------------------------------------------------------------------------*/
XMLElement* CE::GameObjectSerializer::SerializeGameObjectElement(XMLDocument* pDocument, const GameObject* pGameObject)
{
	if (!pGameObject || !pDocument)
		return nullptr;

	XMLElement* pElement = pDocument->NewElement("GameObject");

	// Write attributes
	pElement->SetAttribute("name", pGameObject->GetName().c_str());
	pElement->SetAttribute("tag", pGameObject->GetTag().c_str());

	// Write transform properties
	const Transform& transform = pGameObject->GetTransform();
	Vector2f position = transform.GetPosition();
	Vector2f scale = transform.GetScale();
	float rotation = transform.GetRotation();

	// Format position as "x,y"
	std::ostringstream posStream;
	posStream << std::fixed << std::setprecision(6) << position.x << "," << position.y;
	pElement->SetAttribute("position", posStream.str().c_str());

	// Format scale as "x,y"
	std::ostringstream scaleStream;
	scaleStream << std::fixed << std::setprecision(6) << scale.x << "," << scale.y;
	pElement->SetAttribute("scale", scaleStream.str().c_str());

	// Format rotation
	std::ostringstream rotStream;
	rotStream << std::fixed << std::setprecision(6) << rotation;
	pElement->SetAttribute("rotation", rotStream.str().c_str());

	// Write active state
	pElement->SetAttribute("active", pGameObject->IsActive() ? "true" : "false");

	// Serialize components
	SerializeComponents(pDocument, pElement, pGameObject);

	// Serialize children
	SerializeChildren(pDocument, pElement, pGameObject);

	return pElement;
}

/*-----------------------------------------------------------------------------------------------
| --- SerializeComponents:
-----------------------------------------------------------------------------------------------*/
void CE::GameObjectSerializer::SerializeComponents(XMLDocument* pDocument, XMLElement* pGameObjectElement, const GameObject* pGameObject)
{
	if (!pGameObject || !pDocument || !pGameObjectElement)
		return;

	const auto& components = pGameObject->GetAllComponents();

	for (const auto& pComponent : components)
	{
		if (!pComponent)
			continue;

		const std::string& typeName = pComponent->GetTypeName();

		// Get the component's traits
		const auto* pTypeInfo = ComponentFactory::GetTypeInfo(typeName);
		if (!pTypeInfo)
		{
			CE_LOG("GameObjectSerializer: Unknown component type '{}'", typeName);
			continue;
		}

		// Try to serialize using ComponentTraits
		// We need to dispatch to the correct SerializeComponent based on type name
		// This is done via a template lookup mechanism

		XMLElement* pComponentElement = nullptr;

		// Dispatch to component-specific serialization
		// (This uses a macro-like pattern that gets specialized per component)
		if (typeName == "Transform")
		{
			// Transform is serialized as attributes on GameObject, skip
			continue;
		}
		else if (typeName == "SpriteComponent")
		{
			pComponentElement = ComponentTraits<CE::SpriteComponent>::SerializeComponent(pDocument, pComponent.get());
		}
		else if (typeName == "BoxCollider2D")
		{
			pComponentElement = ComponentTraits<CE::BoxCollider2D>::SerializeComponent(pDocument, pComponent.get());
		}
		else if (typeName == "CircleCollider2D")
		{
			pComponentElement = ComponentTraits<CE::CircleCollider2D>::SerializeComponent(pDocument, pComponent.get());
		}
		else if (typeName == "RigidBody2D")
		{
			pComponentElement = ComponentTraits<CE::RigidBody2D>::SerializeComponent(pDocument, pComponent.get());
		}
		else
		{
			// Try to serialize unknown component with generic fallback
			CE_LOG("GameObjectSerializer: No serialization handler for component type '{}'", typeName);
			continue;
		}

		if (pComponentElement)
		{
			pGameObjectElement->InsertEndChild(pComponentElement);
		}
	}
}

/*-----------------------------------------------------------------------------------------------
| --- SerializeChildren:
-----------------------------------------------------------------------------------------------*/
void CE::GameObjectSerializer::SerializeChildren(XMLDocument* pDocument, XMLElement* pGameObjectElement, const GameObject* pGameObject)
{
	if (!pGameObject || !pDocument || !pGameObjectElement)
		return;

	const auto& children = pGameObject->GetChildren();

	for (const auto& pChild : children)
	{
		if (!pChild)
			continue;

		XMLElement* pChildElement = SerializeGameObjectElement(pDocument, pChild.get());
		if (pChildElement)
		{
			pGameObjectElement->InsertEndChild(pChildElement);
		}
	}
}