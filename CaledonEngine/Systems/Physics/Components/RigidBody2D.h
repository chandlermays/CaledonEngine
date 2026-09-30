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
#include "Utilities/Math/Vector2.h"

// Provides physics movement and other dyanmics, and the ability to attach Collider2D to it

// This is a fundamental physics component that provides multiple simulation dynamics, such as RigidBody2D.position and RigidBody2D.rotation for pose control,
// and RigidBody2D.linearVelocity and RigidBody2D.angularVelocity for velocity control

// You can attach multiple Collider2D to a RigidBody2D to detect collisions and provide a collision response when you set RigidBody2D.bodyType to Dynamic

namespace CE
{
	class PhysicsMaterial2D;

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

		void ApplyConstraintsToVelocity();																							// Zeroes velocity componens hat are frozen by the constraints
		void ClearAccumulators();																									// Clears the accumulated force and torque

	public:
		RigidBody2D();																												// Constructor
		~RigidBody2D() override = default;																							// Destructor
		RigidBody2D(const RigidBody2D&) = delete;																					// Prevents copy-construction
		RigidBody2D& operator=(const RigidBody2D&) = delete;																		// Prevents copy-assignment
		RigidBody2D(RigidBody2D&&) = delete;																						// Prevents move-construction
		RigidBody2D& operator=(RigidBody2D&&) = delete;																				// Prevents move-assignment

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
		void SetSharedMaterial(PhysicsMaterial2D& pMaterial);																		// Sets the shared material of this RigidBody2D

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
		Vector2f GetPointVector(const Vector2f& worldPoint) const;																	// Returns the velocity of the body at a world-space point
		Vector2f GetRelativePointVelocity(const Vector2f& localPoint) const;														// Returns the velocity of the body at a local-space point

		bool IsAwake() const;																										// Returns whether this RigidBody2D is awake or not
		bool IsSleeping() const;																									// Returns whether this RigidBody2D is sleeping or not
		void Sleep();																												// Puts this RigidBody2D to sleep
		void WakeUp();																												// Wakes this RigidBody2D

		// Properties:
		// float angularDamping											:				the angular damping of the RigidBody2D angular velocity
		// float angularVelocity										:				angular velocity in degrees per second
		// BodyType2D bodyType											:				the physical behavior type of the RigidBody2D
		// Vec2 centerOfMass											:				the center of mass of the RigidBody2D in local space
		// int colliderCount											:				returns the number of Collider2D attached to this RigidBody2D
		// CollisionDetectionMode2D collisionDetectionMode				:				the method used by the physics engine to check if two objects have collided
		// BodyConstraints2D constraints								:				controls which degrees of freedom are allowed for the simulation of this RigidBody2D
		// bool freezeRotation											:				controls whether physics will change the rotation of the object
		// float gravityScale											:				the degree to which this object is affected by gravity
		// float inertia												:				the RigidBody2D's resistance to changes in angular velocity (rotation)
		// BodyInterpolation2D interpolation							:				physics interpolation used between updates
		// float linearDamping											:				the linear damping of the RigidBody2D linear velocity
		// Vec2 linearVelocity											:				the liner velocity of the RigidBody2D represents the rate of change over time of the UnnamedClass position in world-units
		// float linearVelocityX										:				the X component of the linear velocity of the RigidBody2D in world-units per second
		// float linearVelocityY										:				the Y component of the linear velocity of the RigidBody2D in world-units per second
		// (ignore) localToWorldMatrix									:				the transformation matrix used to transform the RigidBody2D to world space
		// float mass													:				the mass of the RigidBody2D
		// Vec2 position												:				the position of the RigidBody2D
		// float rotation												:				the rotation of the RigidBody2D
		// PhysicsMaterial2D sharedMaterial								:				the PhysicsMaterial2D that is applied to all Collider2D attached to this RigidBody2D
		// bool simulated												:				indicates whether the RigidBody2D should be simulated or not by the physics system
		// BodySleepMode2D sleepMode									:				the sleep state that the RigidBody2D will initially be in
		// Vec2 totalForce												:				the total amount of force that has been explicitly applied to this RigidBody2D since the last physics simulation step
		// float totalTorque											:				the total amount of torque that has been explicitly applied to this RigidBody2D since the last physics simulation step
		// bool useFullKinematicContacts								:				should kinematic/kinematic and kinematic/static collisions be allowed?
		// Vec2 worldCenterOfMass										:				the center of mass of the RigidBody2D in world space

		// Public Methods:
		// void AddForce(Vec2 force, enum = enum.Force)												:				apply a force to the RigidBody2D
		// void AddForceAtPosition(Vec2 force, Vec2 pos, enum = enum.Force)							:				apply a force at a given position in space
		// void AddForceX(float force, enum = enum.Force)											:				adds a force to the X component of the linearVelocity only leaving the Y component of the world space untouched
		// void AddForceY(float force, enum = enum.Force)											:				adds a force to the Y component of the linearVelocity only leaving the X component of the world space untouched
		// void AddRelativeForce(Vec2 relativeForce, enum = enum.Force)								:				adds a force to the local space linearVelocity (i.e. the force is applied in the rotated coordinate space of the RigidBody2D)
		// void AddRelativeForceX(float force, enum = enum.Force)									:				adds a force to the X component of the linearVelocity in the local space only leaving the Y component of the local space untouched
		// void AddRelativeForceY(float force, enum = enum.Force)									:				adds a force to the Y component of the linearVelocity in the local space only leaving the X component of the local space untouched
		// void AddTorque(float torque, enum = enum.Force)											:				apply a torque to the RigidBody2D's center of mass
		// Vec2 ClosestPoint(Vec2 pos)																:				returns a point on the perimeter of all enabled Colliders attached to this RigidBody2D that is closest to the specified position
		// ColliderDistance2D Distance(Collider2D collider)											:				calculates the minimum distance of this collider against all Collider2D attached to this RigidBody2D
		// Vec2 GetPoint(Vec2 point)																:				get a local space point given the point in global space
		// Vec2 GetPointVelocity(Vec2 point)														:				the velocity of the RigidBody2D at the point in global space
		// Vec2 GetRelativePoint(Vec2 relativePoint)												:				get a global space point given the point in local space
		// Vec2 GetRelativePointVelocity(Vec2 relativePoint)										:				the velocity of the RigidBody2D at the point in local space
		// Vec2 GetVector(Vec2 vector)																:				get a local space vector given the vector in global space
		// Vec2 GetRelativeVector(Vec2 relativeVector)												:				get a global space vector given the vector in local space
		// bool IsAwake()																			:				is the RigidBody2D "awake"?
		// bool IsSleeping()																		:				is the RigidBody2D "sleeping"?
		// bool IsTouching(Collider2D collider)														:				checks whether the collider is touching any of the collider(s) attached to this RigidBody2D or not
		// void MovePosition(Vec2 pos)																:				moves the RigidBody2D to position
		// void MovePositionAndRotation(Vec2 pos, float angle)										:				moves the RigidBody2D to position and rotates by angle
		// void MoveRotation(float angle)															:				rotates the RigidBody2D to the specified angle
		// int Overlap(Vec2 pos, float angle, List<Collider2D> results)								:				get a list of all Colliders that overlap all Colliders attached to this RigidBody2D
		// bool OverlapPoint(Vec2 point)															:				check if any of the RigidBody2D colliders overlap a point in space
		// void SetRotation(float angle)															:				sets the rotation of the RigidBody2D to angle (given in degrees)
		// void Sleep()																				:				make the RigidBody2D "sleep"
		// SlideResults2D Slide(Vec2 velocity, float deltaTime, SlideConfig slideConfig)			:				slide the RigidBody2D using the specified velocity integrated over deltaTime using the configuration specified by SlideConfig.
		// void WakeUp()																			:				disables the "sleeping" state of a RigidBody2D
	};
}