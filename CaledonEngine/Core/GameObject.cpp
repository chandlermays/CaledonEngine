/*------------------------------
| File: GameObject.cpp
| Author: Chandler Mays
------------------------------*/
#include "GameObject.h"

#include "Systems/Engine/LoggingManager.h"
#include "Core/Transform.h"
#include "Core/Component.h"

#include <algorithm>

CE::GameObject::GameObject()
	: m_pParent{ nullptr }
	, m_name{ "GameObject" }
	, m_tag{ "Untagged" }
	, m_isActive{ true }
	, m_pTransform{ nullptr }
{
	auto pTransform = std::make_unique<Transform>();
	m_pTransform = pTransform.get();
	AddComponent(std::move(pTransform));
}

CE::GameObject::~GameObject()
{
	Destroy();
}

bool CE::GameObject::Initialize()
{
	bool success = true;
	for (const auto& pComponent : m_components)
	{
		if (pComponent && !pComponent->Initialize())
		{
			CE_LOG("GameObject::Initialize - A component on '" + m_name + "' failed to initialize.");
			success = false;
		}
	}
	return success;
}

void CE::GameObject::Update(float deltaTime)
{
	if (!m_isActive)
		return;

	for (const auto& pComponent : m_components)
		if (pComponent && pComponent->IsActive())
			pComponent->Update(deltaTime);

	for (const auto& pChild : m_children)
		if (pChild && pChild->IsActive())
			pChild->Update(deltaTime);
}

void CE::GameObject::Render()
{
	if (!m_isActive)
		return;

	for (const auto& pComponent : m_components)
		if (pComponent && pComponent->IsActive())
			pComponent->Render();

	for (const auto& pChild : m_children)
		if (pChild && pChild->IsActive())
			pChild->Render();
}

CE::GameObject* CE::GameObject::GetParent() const { return m_pParent; }
void CE::GameObject::SetParent(GameObject* pParent) { m_pParent = pParent; }

const std::vector<std::unique_ptr<CE::GameObject>>& CE::GameObject::GetChildren() const
{
	return m_children;
}

void CE::GameObject::AddChild(std::unique_ptr<GameObject> pChild)
{
	if (!pChild || pChild.get() == this || pChild->m_pParent != nullptr)
		return;

	pChild->m_pParent = this;
	m_children.emplace_back(std::move(pChild));
}

std::unique_ptr<CE::GameObject> CE::GameObject::RemoveChild(GameObject* pChild)
{
	if (!pChild)
		return nullptr;

	auto it = std::find_if(m_children.begin(), m_children.end(),
		[pChild](const std::unique_ptr<GameObject>& pObject) { return pObject.get() == pChild; });
	if (it == m_children.end())
		return nullptr;

	std::unique_ptr<GameObject> result = std::move(*it);
	m_children.erase(it);
	result->m_pParent = nullptr;
	return result;
}

const std::string& CE::GameObject::GetName() const { return m_name; }
void CE::GameObject::SetName(const std::string& name) { m_name = name; }
const std::string& CE::GameObject::GetTag() const { return m_tag; }
void CE::GameObject::SetTag(const std::string& tag) { m_tag = tag; }
bool CE::GameObject::IsActive() const { return m_isActive; }
void CE::GameObject::SetActive(bool isActive) { m_isActive = isActive; }

const CE::Transform& CE::GameObject::GetTransform() const { return *m_pTransform; }
CE::Transform& CE::GameObject::GetTransform() { return *m_pTransform; }

const std::vector<std::unique_ptr<CE::Component>>& CE::GameObject::GetAllComponents() const
{
	return m_components;
}

void CE::GameObject::AddComponent(std::unique_ptr<CE::Component> pComponent)
{
	if (!pComponent)
		return;

	pComponent->SetOwner(this);
	m_components.emplace_back(std::move(pComponent));
}

std::unique_ptr<CE::Component> CE::GameObject::RemoveComponent(CE::Component* pComponent)
{
	if (!pComponent || pComponent == m_pTransform)
		return nullptr;

	auto it = std::find_if(m_components.begin(), m_components.end(),
		[pComponent](const std::unique_ptr<Component>& pOwned) { return pOwned.get() == pComponent; });
	if (it == m_components.end())
		return nullptr;

	std::unique_ptr<Component> result = std::move(*it);
	m_components.erase(it);
	result->SetOwner(nullptr);
	return result;
}

void CE::GameObject::Destroy()
{
	m_children.clear();
	m_components.clear();
	m_pTransform = nullptr;
	m_pParent = nullptr;
}
