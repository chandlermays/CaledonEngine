/*------------------------------
| File: GameObject.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include <string>
#include <vector>

namespace CE
{
	class Transform;
	class Component;

	class GameObject
	{
	private:
		GameObject* m_pParent;															// Pointer to this GameObject's parent
		std::vector<GameObject*> m_children;											// Vector of pointers to this GameObject's children
		
		std::string m_name;																// The name of this GameObject
		std::string m_tag;																// The tag of this GameObject
		bool m_isActive;																// Whether this GameObject is active or not

		Transform* m_pTransform;														// Pointer to this GameObject's Transform component
		std::vector<Component*> m_components;											// Vector of pointers to Components attached to this GameObject

	public:
		GameObject();																	// Constructor
		~GameObject();																	// Destructor
		GameObject(const GameObject&) = delete;											// Prevent copy-construction
		GameObject& operator=(const GameObject&) = delete;								// Prevent copy-assignment
		GameObject(GameObject&&) = delete;												// Prevent move-construction
		GameObject& operator=(GameObject&&) = delete;									// Prevent move-assignment

		bool Initialize();																// Initializes this GameObject and its components
		void Update(float);																// Updates this GameObject and its components
		void Render();																	// Renders this GameObject and its components

		GameObject* GetParent() const;													// Returns a pointer to this GameObject's parent
		void SetParent(GameObject* pParent);											// Sets the parent of this GameObject

		const std::vector<GameObject*>& GetChildren() const;							// Returns a vector of pointers to this GameObject's children
		void AddChild(GameObject* pChild);												// Adds a child to this GameObject
		void RemoveChild(GameObject* pChild);											// Removes a child from this GameObject

		const std::string& GetName() const;												// Returns the name of this GameObject
		void SetName(const std::string& name);											// Sets the name of this GameObject

		const std::string& GetTag() const;												// Returns the tag of this GameObject
		void SetTag(const std::string& tag);											// Sets the tag of this GameObject

		bool IsActive() const;															// Returns whether this GameObject is active or not
		void SetActive(bool isActive);													// Sets whether this GameObject is active or not

		const Transform& GetTransform() const;											// Returns the Transform of this GameObject
		Transform& GetTransform();														// Returns the Transform of this GameObject

		// Returns a reference to a component of type T attached to this GameObject
		template<typename T>
		T* GetComponent() const;

		// Returns a reference to a component of type T attached to this GameObject, or any of its children
		template<typename T>
		T* GetComponentInChildren() const;

		// Returns a reference to a component of type T attached to this GameObject, or any of its parents
		template<typename T>
		T* GetComponentInParent() const;

		// Returns references to all components of type T attached to this GameObject
		template<typename T>
		std::vector<T*> GetComponents() const;

		// Returns references to all components of type T attached to this GameObject and its children
		template<typename T>
		std::vector<T*> GetComponentsInChildren() const;

		// Returns references to all components of type T attached to this GameObject and its parents
		template<typename T>
		std::vector<T*> GetComponentsInParent() const;

		void AddComponent(Component* pComponent);										// Adds a component to this GameObject
		void RemoveComponent(Component* pComponent);									// Removes a component from this GameObject

		void Destroy();																	// Destroys this GameObject and its components
	};

	/*------------------------------------------------------------------------------------------------
	| --- GetComponent: Returns a reference to a component of type T attached to this GameObject --- |
	------------------------------------------------------------------------------------------------*/
	template<typename T>
	inline T* GameObject::GetComponent() const
	{
		for (Component* pComponent : m_components)
		{
			T* pResult = dynamic_cast<T*>(pComponent);		// NOTE: Keep the use of 'dynamic_cast' until performance becomes an issue
			if (pResult != nullptr)
			{
				return pResult;
			}
		}
		return nullptr;
	}

	/*----------------------------------------------------------------------------------------------------------------------------------
	| --- GetComponentInChildren: Returns a reference to a component of type T attached to this GameObject, or any of its children --- |
	----------------------------------------------------------------------------------------------------------------------------------*/
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

	/*-------------------------------------------------------------------------------------------------------------------------------
	| --- GetComponentInParent: Returns a reference to a component of type T attached to this GameObject, or any of its parents --- |
	-------------------------------------------------------------------------------------------------------------------------------*/
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

	/*---------------------------------------------------------------------------------------------------
	| --- GetComponents: Returns references to all components of type T attached to this GameObject --- |
	---------------------------------------------------------------------------------------------------*/
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

	/*------------------------------------------------------------------------------------------------------------------------------
	| --- GetComponentsInChildren: Returns references to all components of type T attached to this GameObject and its children --- |
	------------------------------------------------------------------------------------------------------------------------------*/
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

	/*---------------------------------------------------------------------------------------------------------------------------
	| --- GetComponentsInParent: Returns references to all components of type T attached to this GameObject and its parents --- |
	---------------------------------------------------------------------------------------------------------------------------*/
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