#pragma once
#include <string>
#include <vector>

namespace CE
{
	class GameObject;
	class Transform;

	class Component
	{
	protected:
		GameObject* m_pOwner;																// Pointer to the owning GameObject
		std::string m_id;																	// The unique identifier of the Component
		bool m_isActive;																	// Whether the Component is active or not

	public:
		Component();																		// Constructor
		virtual ~Component() {};															// Destructor
		Component(const Component&) = delete;												// Prevent copy-construction
		Component& operator=(const Component&) = delete;									// Prevent copy-assignment
		Component(Component&&) = delete;													// Prevent move-construction
		Component& operator=(Component&&) = delete;											// Prevent move-assignment

		virtual bool Initialize() { return true; }											// Initializes the Component
		virtual void Update(float) {};														// Updates the Component, if applicable
		virtual void Render() {};															// Renders the Component, if applicable

		GameObject* GetOwner() const;														// Returns a pointer to the owning GameObject
		void SetOwner(GameObject* owner);													// Sets the owning GameObject

		const std::string& GetID() const;													// Returns the unique identifier of the Component
		void SetID(const std::string& id);													// Sets the unique identifier of the Component

		bool IsActive() const;																// Returns whether the Component is active or not
		void SetActive(bool isActive);														// Sets whether the Component is active or not

		// Returns a reference to a component of type T attached to the same GameObject
		template<typename T>
		T* GetComponent() const;

		// Returns a reference to a component of type T attached to same GameObject, or any of its children
		template<typename T>
		T* GetComponentInChildren() const;

		// Returns a reference to a component of type T attached to same GameObject, or any of its parents
		template<typename T>
		T* GetComponentInParent() const;

		// Returns references to all components of type T attached to the same GameObject
		template<typename T>
		std::vector<T*> GetComponents() const;

		// Returns references to all components of type T attached to same GameObject, or any of its children
		template<typename T>
		std::vector<T*> GetComponentsInChildren() const;

		// Returns references to all components of type T attached to same GameObject, or any of its parents
		template<typename T>
		std::vector<T*> GetComponentsInParent() const;
	};

	/*----------------------------------------------------------------------------------------------------
	| --- GetComponent: Returns a reference to a component of type T attached to the same GameObject --- |
	----------------------------------------------------------------------------------------------------*/
	template<typename T>
	inline T* Component::GetComponent() const
	{
		//...
	}

	/*----------------------------------------------------------------------------------------------------------------------------------
	| --- GetComponentInChildren: Returns a reference to a component of type T attached to same GameObject, or any of its children --- |
	----------------------------------------------------------------------------------------------------------------------------------*/
	template<typename T>
	inline T* Component::GetComponentInChildren() const
	{
		//...
	}

	/*-------------------------------------------------------------------------------------------------------------------------------
	| --- GetComponentInParent: Returns a reference to a component of type T attached to same GameObject, or any of its parents --- |
	-------------------------------------------------------------------------------------------------------------------------------*/
	template<typename T>
	inline T* Component::GetComponentInParent() const
	{
		//...
	}

	/*-------------------------------------------------------------------------------------------------------
	| --- GetComponents: Returns references to all components of type T attached to the same GameObject --- |
	-------------------------------------------------------------------------------------------------------*/
	template<typename T>
	inline std::vector<T*> Component::GetComponents() const
	{
		//...
	}

	/*------------------------------------------------------------------------------------------------------------------------------
	| --- GetComponentsInChildren: Returns references to all components of type T attached to same GameObject and its children --- |
	------------------------------------------------------------------------------------------------------------------------------*/
	template<typename T>
	inline std::vector<T*> Component::GetComponentsInChildren() const
	{
		//...
	}

	/*---------------------------------------------------------------------------------------------------------------------------
	| --- GetComponentsInParent: Returns references to all components of type T attached to same GameObject and its parents --- |
	---------------------------------------------------------------------------------------------------------------------------*/
	template<typename T>
	inline std::vector<T*> Component::GetComponentsInParent() const
	{
		//...
	}
}