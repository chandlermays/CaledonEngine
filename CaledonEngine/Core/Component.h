#pragma once
#include <string>

namespace CE
{
	class GameObject;

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

		virtual bool Initialize() { return true; };
		virtual void Update(float) {};
		virtual void Render() {};

		GameObject* GetOwner() const { return m_pOwner; }
		void SetOwner(GameObject* owner) { m_pOwner = owner; }

		const std::string& GetID() const { return m_id; }
		void SetID(const std::string& id) { m_id = id; }

		bool IsActive() const { return m_isActive; }
		void SetActive(bool isActive) { m_isActive = isActive; }
	};
}