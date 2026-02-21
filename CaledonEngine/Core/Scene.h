#pragma once
#include <string>
#include <vector>

namespace CE
{
	class GameObject;

	class Scene
	{
	private:
		std::string m_name;														// The name of this Scene
		std::vector<GameObject*> m_gameObjects;									// The GameObjects in this Scene
		bool m_isActive;														// Whether this Scene is active or not
		 bool m_isDirty;														// Whether this Scene has unsaved changes (needs to be updated/rendered)
		 bool m_isLoaded;														// Whether this Scene has completed loading and objects have been initialized/enabled

	public:
		Scene();																// Constructor
		~Scene();																// Destructor
		Scene(const Scene&) = delete;											// Prevent copy-construction
		Scene& operator=(const Scene&) = delete;								// Prevent copy-assignment
		Scene(Scene&&) = delete;												// Prevent move-construction
		Scene& operator=(Scene&&) = delete;										// Prevent move-assignment

		bool Initialize();														// Initializes this Scene and its GameObjects
		void Update(float);														// Updates this Scene and its GameObjects
		void Render();															// Renders this Scene and its GameObjects

		const std::string& GetName() const;										// Returns the name of this Scene
		void SetName(const std::string& name);									// Sets the name of this Scene

		bool IsActive() const;													// Returns whether this Scene is active or not
		void SetActive(bool isActive);											// Sets whether this Scene is active or not

		bool IsValid() const;													// Checks if this Scene is valid

		void AddGameObject(GameObject* pGameObject);							// Adds a GameObject to this Scene
		void RemoveGameObject(GameObject* pGameObject);							// Removes a GameObject from this Scene
		const std::vector<GameObject*>& GetGameObjects() const;					// Returns the GameObjects in this Scene

		void Clear();															// Clears all GameObjects from this Scene
	};
}