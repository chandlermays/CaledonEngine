#include "GameObject.h"

CE::GameObject::GameObject()
{
}

CE::GameObject::~GameObject()
{
}

GameObject* CE::GameObject::GetParent() const
{
    return nullptr;
}

void CE::GameObject::SetParent(GameObject* parent)
{
}

const std::vector<GameObject*>& CE::GameObject::GetChildren() const
{
    // TODO: insert return statement here
}

void CE::GameObject::AddChild(GameObject* child)
{
}

void CE::GameObject::RemoveChild(GameObject* child)
{
}

const std::string& CE::GameObject::GetName() const
{
    // TODO: insert return statement here
}

void CE::GameObject::SetName(const std::string& name)
{
}

const std::string& CE::GameObject::GetTag() const
{
    // TODO: insert return statement here
}

void CE::GameObject::SetTag(const std::string& tag)
{
}
