#pragma once
#include "CaledonEngine/Systems/Engine/Manager.h"

namespace CE
{
	class CollisionManager : public Manager
	{
	private:


	public:
		CollisionManager();														// Constructor
		~CollisionManager();													// Destructor
		CollisionManager(const CollisionManager&) = delete;						// Prevent copy-construction
		CollisionManager& operator=(const CollisionManager&) = delete;			// Prevent copy-assignment
		CollisionManager(CollisionManager&&) = delete;							// Prevent move-construction
		CollisionManager& operator=(CollisionManager&&) = delete;				// Prevent move-assignment

		bool Initialize() override;												// Initialize the collision manager
		void Update(float deltaTime) override;									// Update the collision manager
		void Render() override;													// Render the current frame
		void Shutdown() override;												// Shutdown the collision manager
	};
}