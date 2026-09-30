/*------------------------------
| File: RigidBody2D.h
| Author: Chandler Mays
------------------------------*/
#pragma once

#include "Core/Component.h"
#include "Systems/Physics/Types/BodyType2D.h"
#include "Systems/Physics/Types/BodyConstraints2D.h"
#include "Systems/Physics/Types/BodyInterpolation2D.h"
#include "Systems/Physics/Types/BodySleepMode2D.h"
#include "Systems/Physics/Types/CollisionDetectionMode2D.h"
#include "Systems/Physics/Types/ForceMode2D.h"
#include "Systems/Physics/Types/SlideConfig2D.h"
#include "Systems/Physics/Types/SlideResults2D.h"
#include "Utilities/Math/Vector2.h"

// Provides physics movement and other dyanmics, and the ability to attach Collider2D to it

// This is a fundamental physics component that provides multiple simulation dynamics, such as RigidBody2D.position and RigidBody2D.rotation for pose control,
// and RigidBody2D.linearVelocity and RigidBody2D.angularVelocity for velocity control

// You can attach multiple Collider2D to a RigidBody2D to detect collisions and provide a collision response when you set RigidBody2D.bodyType to Dynamic

namespace CE
{
	class PhysicsMaterial2D;
	class CollisionManager;

	class RigidBody2D : public Component
	{
	private:
		BodyType2D m_bodyType;																										// The physical behavior type of this RigidBody2D
		BodyConstraints2D m_constraints;																							// Controls which degrees of freedom are allowed for the simulation of this RigidBody2D
		BodyInterpolation2D m_interpolation;																						// The physics interpolation used between updates
		BodySleepMode2D m_sleepMode;																								// The sleep state that this RigidBody2D will initially be in
		CollisionDetectionMode2D m_collisionDetectionMode;																			// The method used by the physics engine to check if two objects have collided

		Vector2f m_linearVelocity;																									// The linear velocity in world-units per second
		float m_angularVelocity;																									// The angular velocity in degrees per second
		float m_linearDamping;																										// The damping applied to linear velocity
		float m_angularDamping;																										// The damping applied to angular velocity
		float m_gravityScale;																										// The degree to which this object is affected by gravity
		float m_mass;																												// The mass of this RigidBody2D
		float m_inertia;																											// The resistance to changes in angular velocity
		Vector2f m_centerOfMass;																									// The center of mass in local space
		
		Vector2f m_totalForce;																										// The force applied since the last physics step
		float m_totalTorque;																										// The torque applied since the last physics step

		bool m_isSimulated;																											// Whether the physics system simulated this RigidBody2D
		bool m_useFullKinematicContacts;																							// Whether kinematic/kinematic and kinematic/static contacts are allowed
		bool m_isSleeping;																											// Whether this RigidBody2D is currently asleep

		PhysicsMaterial2D* m_pSharedMaterial;																						// The Material applied to all attached colliders
		CollisionManager* m_pCollisionManager;																						// Pointer to the Collision Manager

		void ApplyConstraintsToVelocity();																							// Zeroes velocity componens hat are frozen by the constraints
		void ClearAccumulators();																									// Clears the accumulated force and torque

	public:
		RigidBody2D();																												// Constructor
		~RigidBody2D() override;																									// Destructor
		RigidBody2D(const RigidBody2D&) = delete;																					// Prevents copy-construction
		RigidBody2D& operator=(const RigidBody2D&) = delete;																		// Prevents copy-assignment
		RigidBody2D(RigidBody2D&&) = delete;																						// Prevents move-construction
		RigidBody2D& operator=(RigidBody2D&&) = delete;																				// Prevents move-assignment

		bool Initialize() override;																									// Prepares the RigidBody2D for use

		const std::string& GetTypeName() const override;																			// Returns the type name of this Component

		void Step(float deltaTime, const Vector2f& gravity);																		// Integrates forces, gravity, damping, and velocity into the Transform for one physics step

		BodyType2D GetBodyType() const;																								// Returns the body type of this RigidBody2D
		void SetBodyType(BodyType2D type);																							// Sets the body type of this RigidBody2D

		BodyConstraints2D GetConstraints() const;																					// Returns the constraints of this RigidBody2D
		void SetConstraints(BodyConstraints2D constraints);																			// Sets the constraints of this RigidBody2D

		bool GetFreezeRotation() const;																								// Returns whether the rotation is frozen
		void SetFreezeRotation(bool freee);																							// Sets whether the rotation is to be frozen or not

		BodyInterpolation2D GetInterpolation() const;																				// Returns the interpolation of this RigidBody2D
		void SetInterpolation(BodyInterpolation2D mode);																			// Sets the interpolation of this RigidBody2D

		BodySleepMode2D GetSleepMode() const;																						// Returns the sleep mode of this RigidBody2D
		void SetSleepMode(BodySleepMode2D mode);																					// Sets the sleep mode of this RigidBody2D

		CollisionDetectionMode2D GetCollisionDetectionMode() const;																	// Returns the collision detection mode of this RigidBody2D
		void SetCollisionDetectionMode(CollisionDetectionMode2D mode);																// Sets the collision detection mode of this RigidBody2D

		const Vector2f& GetLinearVelocity() const;																					// Returns the linear velocity of this RigidBody2D
		void SetLinearVelocity(const Vector2f& velocity);																			// Sets the linear velocity of this RigidBody2D

		float GetLinearVelocityX() const;																							// Returns the X component of the linear velocity
		void SetLinearVelocityX(float x);																							// Sets the X component of the linear velocity

		float GetLinearVelocityY() const;																							// Returns the Y component of the linear velocity
		void SetLinearVelocityY(float y);																							// Sets the Y compoennt of the linear velocity

		float GetAngularVelocity() const;																							// Returns the angular velociy of this RigidBody2D
		void SetAngularVelocity(float velocity);																					// Sets the angular velocity of this RigidBody2D

		float GetLinearDamping() const;																								// Returns the damping applied to the linear velocity
		void SetLinearDamping(float damping);																						// Sets the damping applied to the linear velocity

		float GetAngularDamping() const;																							// Returns the damping applied to the angular velocity
		void SetAngularDamping(float damping);																						// Sets the damping applied to the angular velocity

		float GetGravityScale() const;																								// Returns the gravity scale
		void SetGravityScale(float scale);																							// Sets the gravity scale

		float GetMass() const;																										// Returns the mass of this RigidBody2D
		void SetMass(float mass);																									// Sets the mass of this RigidBody2D

		float GetInertia() const;																									// Returns the inertia of this RigidBody2D
		void SetInertia(float inertia);																								// Sets the ineria of this RigidBody2D

		const Vector2f& GetCenterOfMass() const;																					// Returns the center of mass of this RigidBody2D in local space
		void SetCenterOfMass(const Vector2f& centerOfMass);																			// Sets the center of mass of this RigidBody2D in local space

		Vector2f GetWorldCenterOfMass() const;																						// Returns the center of mass of this RigidBody2D in world space

		Vector2f GetPosition() const;																								// Returns the position of this RigidBody2D
		void SetPosition(const Vector2f& position);																					// Sets the position of this RigidBody2D

		float GetRotation() const;																									// Returns the rotation of this RigidBody2D
		void SetRotation(float angle);																								// Sets the rotation of this RigidBody2D

		bool IsSimulated() const;																									// Returns whether this RigidBody2D is simulated or not
		void SetSimulated(bool simulated);																							// Sets whether this RigidBody2D is to be simulated or not

		bool GetUseFullKinematicContacts() const;																					// Returns whether kinematic/kinematic and kinematic/static contacts are allowed or not
		void SetUseFullKinematicContacts(bool use);																					// Sets whether kinematic/kinematic and kinematic/static contacts are allowed or not

		PhysicsMaterial2D* GetSharedMaterial() const;																				// Returns the shared material of this RigidBody2D
		void SetSharedMaterial(PhysicsMaterial2D* pMaterial);																		// Sets the shared material of this RigidBody2D

		int GetColliderCount() const;																								// Returns the number of Collider2D on the same GameObject

		const Vector2f& GetTotalForce() const;																						// Returns the force applied since the last physics step
		float GetTotalTorque() const;																								// Returns the torque applied since the last physics step

		void AddForce(const Vector2f& force, ForceMode2D mode = ForceMode2D::Force);												// Applies a force (or instant impulse) in world space
		void AddForceX(float force, ForceMode2D mode = ForceMode2D::Force);															// Applies a world-space force along X only
		void AddForceY(float force, ForceMode2D mode = ForceMode2D::Force);															// Applies a world-space force along Y only
		void AddRelativeForce(const Vector2f& relativeForce, ForceMode2D mode = ForceMode2D::Force);								// Applies a force in this RigidBody2D's rotated (local) space
		void AddRelativeForceX(float force, ForceMode2D mode = ForceMode2D::Force);													// Applies a local-space force along X only
		void AddRelativeForceY(float force, ForceMode2D mode = ForceMode2D::Force);													// Applies a local-space force along Y only
		void AddForceAtPosition(const Vector2f& force, const Vector2f& position, ForceMode2D mode = ForceMode2D::Force);			// Applies a force at a world position
		void AddTorque(float torque, ForceMode2D mode = ForceMode2D::Force);														// Applies a torque about the center of mass
		
		Vector2f GetPoint(const Vector2f& worldPoint) const;																		// Converts a world-space point to local space
		Vector2f GetRelativePoint(const Vector2f& localPoint) const;																// Converts a local-space point to world space
		Vector2f GetVector(const Vector2f& worldVector) const;																		// Converts a world-space vector to local space
		Vector2f GetRelativeVector(const Vector2f& localVector) const;																// Converts a local-space vector to world space
		Vector2f GetPointVelocity(const Vector2f& worldPoint) const;																// Returns the velocity of the body at a world-space point
		Vector2f GetRelativePointVelocity(const Vector2f& localPoint) const;														// Returns the velocity of the body at a local-space point

		SlideResults2D Slide(const Vector2f& velocity, float deltaTime, const SlideConfig2D& config);								// Slide this RigidBody2D using the specified velocity and configuration integrated over deltaTime

		bool IsAwake() const;																										// Returns whether this RigidBody2D is awake or not
		bool IsSleeping() const;																									// Returns whether this RigidBody2D is sleeping or not
		void Sleep();																												// Puts this RigidBody2D to sleep
		void WakeUp();																												// Wakes this RigidBody2D

		// Public Methods:
		// Vec2 ClosestPoint(Vec2 pos)																:				returns a point on the perimeter of all enabled Colliders attached to this RigidBody2D that is closest to the specified position
		// ColliderDistance2D Distance(Collider2D collider)											:				calculates the minimum distance of this collider against all Collider2D attached to this RigidBody2D
		// void MovePosition(Vec2 pos)																:				moves the RigidBody2D to position
		// void MovePositionAndRotation(Vec2 pos, float angle)										:				moves the RigidBody2D to position and rotates by angle
		// void MoveRotation(float angle)															:				rotates the RigidBody2D to the specified angle
		// int Overlap(Vec2 pos, float angle, List<Collider2D> results)								:				get a list of all Colliders that overlap all Colliders attached to this RigidBody2D
		// bool OverlapPoint(Vec2 point)															:				check if any of the RigidBody2D colliders overlap a point in space
	};
}