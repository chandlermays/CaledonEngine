/*------------------------------
| File: GameObject.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include <memory>
#include <string>
#include <vector>

namespace CE
{
	class Transform;
	class Component;

	class GameObject
	{
	private:
		GameObject* m_pParent; // Non-owning pointer to this GameObject's parent
		std::vector<std::unique_ptr<GameObject>> m_children; // Owned child GameObjects

		std::string m_name;
		std::string m_tag;
		bool m_isActive;

		Transform* m_pTransform; // Non-owning alias to the Transform owned by m_components
		std::vector<std::unique_ptr<Component>> m_components; // Owned Components

	public:
		GameObject();
		~GameObject();
		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject(GameObject&&) = delete;
		GameObject& operator=(GameObject&&) = delete;

		bool Initialize();
		void Update(float);
		void Render();

		GameObject* GetParent() const;
		void SetParent(GameObject* pParent); // Non-owning relationship helper

		const std::vector<std::unique_ptr<GameObject>>& GetChildren() const;
		void AddChild(std::unique_ptr<GameObject> pChild);
		std::unique_ptr<GameObject> RemoveChild(GameObject* pChild);

		const std::string& GetName() const;
		void SetName(const std::string& name);
		const std::string& GetTag() const;
		void SetTag(const std::string& tag);
		bool IsActive() const;
		void SetActive(bool isActive);

		const Transform& GetTransform() const;
		Transform& GetTransform();

		template<typename T> T* GetComponent() const;
		template<typename T> T* GetComponentInChildren() const;
		template<typename T> T* GetComponentInParent() const;
		template<typename T> std::vector<T*> GetComponents() const;
		template<typename T> std::vector<T*> GetComponentsInChildren() const;
		template<typename T> std::vector<T*> GetComponentsInParent() const;

		const std::vector<std::unique_ptr<Component>>& GetAllComponents() const;
		void AddComponent(std::unique_ptr<Component> pComponent);
		std::unique_ptr<Component> RemoveComponent(Component* pComponent);

		void Destroy();
	};

	template<typename T>
	inline T* GameObject::GetComponent() const
	{
		for (const auto& pComponent : m_components)
		{
			if (T* pResult = dynamic_cast<T*>(pComponent.get()))
				return pResult;
		}
		return nullptr;
	}

	template<typename T>
	inline T* GameObject::GetComponentInChildren() const
	{
		if (T* pComponent = GetComponent<T>())
			return pComponent;

		for (const auto& pChild : m_children)
		{
			if (T* pComponent = pChild->GetComponentInChildren<T>())
				return pComponent;
		}
		return nullptr;
	}

	template<typename T>
	inline T* GameObject::GetComponentInParent() const
	{
		if (T* pComponent = GetComponent<T>())
			return pComponent;
		return m_pParent ? m_pParent->GetComponentInParent<T>() : nullptr;
	}

	template<typename T>
	inline std::vector<T*> GameObject::GetComponents() const
	{
		std::vector<T*> components;
		for (const auto& pComponent : m_components)
		{
			if (T* pResult = dynamic_cast<T*>(pComponent.get()))
				components.emplace_back(pResult);
		}
		return components;
	}

	template<typename T>
	inline std::vector<T*> GameObject::GetComponentsInChildren() const
	{
		std::vector<T*> components = GetComponents<T>();
		for (const auto& pChild : m_children)
		{
			std::vector<T*> childComponents = pChild->GetComponentsInChildren<T>();
			components.insert(components.end(), childComponents.begin(), childComponents.end());
		}
		return components;
	}

	template<typename T>
	inline std::vector<T*> GameObject::GetComponentsInParent() const
	{
		std::vector<T*> components = GetComponents<T>();
		if (m_pParent)
		{
			std::vector<T*> parentComponents = m_pParent->GetComponentsInParent<T>();
			components.insert(components.end(), parentComponents.begin(), parentComponents.end());
		}
		return components;
	}
}
