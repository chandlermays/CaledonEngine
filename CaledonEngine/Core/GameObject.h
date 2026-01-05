#pragma once
#include "Transform.h"
#include <string>
#include <vector>

namespace CE
{
	class Component;

	class GameObject
	{
	private:
		GameObject* m_pParent;															// Pointer to the parent GameObject
		std::vector<GameObject*> m_children;											// Vector of pointers to child GameObjects
		
		std::string m_name;																// The name of the GameObject
		std::string m_tag;																// The tag of the GameObject
		bool m_isActive;																// Whether the GameObject is active or not

		Transform m_transform;															// The Transform of the GameObject
		std::vector<Component*> m_components;											// Vector of pointers to Components attached to the GameObject

	public:
		GameObject();																	// Constructor
		~GameObject();																	// Destructor
		GameObject(const GameObject&) = delete;											// Prevent copy-construction
		GameObject& operator=(const GameObject&) = delete;								// Prevent copy-assignment
		GameObject(GameObject&&) = delete;												// Prevent move-construction
		GameObject& operator=(GameObject&&) = delete;									// Prevent move-assignment

		bool Initialize();																// Initializes the GameObject and its components
		void Update(float);																// Updates the GameObject and its components
		void Render();																	// Renders the GameObject and its components

		GameObject* GetParent() const;													// Gets a pointer to the parent GameObject
		void SetParent(GameObject* parent);												// Sets the parent GameObject

		const std::vector<GameObject*>& GetChildren() const;							// Gets a vector of pointers to child GameObjects
		void AddChild(GameObject* child);												// Adds a child GameObject
		void RemoveChild(GameObject* child);											// Removes a child GameObject

		const std::string& GetName() const;												// Gets the name of the GameObject
		void SetName(const std::string& name);											// Sets the name of the GameObject

		const std::string& GetTag() const;												// Gets the tag of the GameObject
		void SetTag(const std::string& tag);											// Sets the tag of the GameObject

		bool IsActive() const;															// Gets whether the GameObject is active or not
		void SetActive(bool isActive);													// Sets whether the GameObject is active or not

		const Transform& GetTransform() const;											// Gets the Transform of the GameObject

		// Gets a reference to a component of type T attached to this GameObject
		template<typename T>
		T* GetComponent() const;

		// Gets a reference to a component of type T attached to this GameObject, or any of its children
		template<typename T>
		T* GetComponentInChildren() const;

		// Gets a reference to a component of type T attached to this GameObject, or any of its parents
		template<typename T>
		T* GetComponentInParent() const;

		// Gets references to all components of type T attached to this GameObject
		template<typename T>
		std::vector<T*> GetComponents() const;

		// Gets references to all components of type T attached to this GameObject and its children
		template<typename T>
		std::vector<T*> GetComponentsInChildren() const;

		// Gets references to all components of type T attached to this GameObject and its parents
		template<typename T>
		std::vector<T*> GetComponentsInParent() const;

		void AddComponent(Component* component);										// Adds a component to the GameObject
		void RemoveComponent(Component* component);										// Removes a component from the GameObject

		void Destroy();																	// Destroys the GameObject and its components
	};

	/*---------------------------------------------------------------------------------------------
	| --- GetComponent: Gets a reference to a component of type T attached to this GameObject --- |
	---------------------------------------------------------------------------------------------*/
	template<typename T>
	inline T* GameObject::GetComponent() const
	{
		for (Component* pComponent : m_components)
		{
			T* pResult = dynamic_cast<T*>(pComponent);		// Keep the use of 'dynamic_cast' until performance becomes an issue
			if (pResult != nullptr)
			{
				return pResult;
			}
		}
		return nullptr;
	}

	/*-------------------------------------------------------------------------------------------------------------------------------
	| --- GetComponentInChildren: Gets a reference to a component of type T attached to this GameObject, or any of its children --- |
	-------------------------------------------------------------------------------------------------------------------------------*/
	template<typename T>
	inline T* GameObject::GetComponentInChildren() const
	{
		// Check this GameObject first
		T* pComponent = GetComponent<T>();
		if (pComponent != nullptr)
		{
			return pComponent;
		}

		// Otherwise, check children recursively
		for (GameObject* pChild : m_children)
		{
			pComponent = pChild->GetComponentInChildren<T>();
			if (pComponent != nullptr)
			{
				return pComponent;
			}
		}

		// Not found
		return nullptr;
	}

	/*----------------------------------------------------------------------------------------------------------------------------
	| --- GetComponentInParent: Gets a reference to a component of type T attached to this GameObject, or any of its parents --- |
	----------------------------------------------------------------------------------------------------------------------------*/
	template<typename T>
	inline T* GameObject::GetComponentInParent() const
	{
		// Check this GameObject first
		T* pComponent = GetComponent<T>();
		if (pComponent != nullptr)
		{
			return pComponent;
		}

		// Otherwise, check parent recursively
		if (m_pParent != nullptr)
		{
			return m_pParent->GetComponentInParent<T>();
		}

		// Not found
		return nullptr;
	}

	/*------------------------------------------------------------------------------------------------
	| --- GetComponents: Gets references to all components of type T attached to this GameObject --- |
	------------------------------------------------------------------------------------------------*/
	template<typename T>
	inline std::vector<T*> GameObject::GetComponents() const
	{
		std::vector<T*> components;

		for (Component* pComponent : m_components)
		{
			T* pResult = dynamic_cast<T*>(pComponent);		// Keep the use of 'dynamic_cast' until performance becomes an issue
			if (pResult != nullptr)
			{
				components.emplace_back(pResult);
			}
		}
		return components;
	}

	/*---------------------------------------------------------------------------------------------------------------------------
	| --- GetComponentsInChildren: Gets references to all components of type T attached to this GameObject and its children --- |
	---------------------------------------------------------------------------------------------------------------------------*/
	template<typename T>
	inline std::vector<T*> GameObject::GetComponentsInChildren() const
	{
		std::vector<T*> components;

		// Check this GameObject first
		for (Component* pComponent : m_components)
		{
			T* pResult = dynamic_cast<T*>(pComponent);
			if (pResult != nullptr)
			{
				components.emplace_back(pResult);
			}
		}

		// Then, check children recursively
		for (GameObject* pChild : m_children)
		{
			std::vector<T*> childComponents = pChild->GetComponentsInChildren<T>();
			components.insert(components.end(), childComponents.begin(), childComponents.end());
		}

		return components;
	}

	/*------------------------------------------------------------------------------------------------------------------------
	| --- GetComponentsInParent: Gets references to all components of type T attached to this GameObject and its parents --- |
	------------------------------------------------------------------------------------------------------------------------*/
	template<typename T>
	inline std::vector<T*> GameObject::GetComponentsInParent() const
	{
		std::vector<T*> components;

		// Check this GameObject first
		for (Component* pComponent : m_components)
		{
			T* pResult = dynamic_cast<T*>(pComponent);
			if (pResult != nullptr)
			{
				components.emplace_back(pResult);
			}
		}

		// Then, check parent recursively
		if (m_pParent != nullptr)
		{
			std::vector<T*> parentComponents = m_pParent->GetComponentsInParent<T>();
			components.insert(components.end(), parentComponents.begin(), parentComponents.end());
		}

		return components;
	}
}