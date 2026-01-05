#pragma once
#include <string>
#include <vector>
#include "GameObject.h"

namespace CE
{
	class GameObject;
	class Transform;

	class Component
	{
	protected:
		GameObject* m_pOwner;
		std::string m_id;
		bool m_isActive;

	public:
		Component()
			: m_pOwner{ nullptr }
			, m_id{ "" }
			, m_isActive{ true }
		{}

		virtual ~Component() {};
		Component(const Component&) = delete;
		Component& operator=(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator=(Component&&) = delete;

		virtual bool Initialize() {};
		virtual void Update(float) {};
		virtual void Render() {};

		GameObject* GetOwner() const { return m_pOwner; }
		void SetOwner(GameObject* owner) { m_pOwner = owner; }

		const std::string& GetID() const { return m_id; }
		void SetID(const std::string& id) { m_id = id; }

		bool IsActive() const { return m_isActive; }
		void SetActive(bool isActive) { m_isActive = isActive; }

		// Gets a reference to a component of type T attached to the same GameObject
		template<typename T>
		T* GetComponent() const;

		// Gets a reference to a component of type T attached to same GameObject, or any of its children
		template<typename T>
		T* GetComponentInChildren() const;

		// Gets a reference to a component of type T attached to same GameObject, or any of its parents
		template<typename T>
		T* GetComponentInParent() const;

		// Gets references to all components of type T attached to the same GameObject
		template<typename T>
		std::vector<T*> GetComponents() const;

		// Gets references to all components of type T attached to same GameObject, or any of its children
		template<typename T>
		std::vector<T*> GetComponentsInChildren() const;

		// Gets references to all components of type T attached to same GameObject, or any of its parents
		template<typename T>
		std::vector<T*> GetComponentsInParent() const;
	};
}