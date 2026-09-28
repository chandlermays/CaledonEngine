/*------------------------------
| File: RigidBody2D.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "Core/Component.h"

namespace CE
{
	class RigidBody2D : public Component
	{
		// Provides physics movement and other dyanmics, and the ability to attach Collider2D to it
		
		// This is a fundamental physics component that provides multiple simulation dynamics, such as RigidBody2D.position and RigidBody2D.rotation for pose control,
		// and RigidBody2D.linearVelocity and RigidBody2D.angularVelocity for velocity control
		
		// You can attach multiple Collider2D to a RigidBody2D to detect collisions and provide a collision response when you set RigidBody2D.bodyType to Dynamic
		
		// Constructor
		// Destructor

		// Properties:
		// angularDamping					:				the angular damping of the RigidBody2D angular velocity
		// angularVelocity					:				angular velocity in degrees per second
		// bodyType							:				the physical behavior type of the RigidBody2D
		// centerOfMass						:				the center of mass of the RigidBody2D in local space
		// colliderCount					:				returns the number of Collider2D attached to this RigidBody2D
		// collisionDetectionMode			:				the method used by the physics engine to check if two objects have collided
		// constraints						:				controls which degrees of freedom are allowed for the simulation of this RigidBody2D
		// freezeRotation					:				controls whether physics will change the rotation of the object
		// gravityScale						:				the degree to which this object is affected by gravity
		// inertia							:				the RigidBody2D's resistance to changes in angular velocity (rotation)
		// interpolation					:				physics interpolation used between updates
		// linearDamping					:				the linear damping of the RigidBody2D linear velocity
		// linearVelocity					:				the liner velocity of the RigidBody2D represents the rate of change over time of the UnnamedClass position in world-units
		// linearVelocityX					:				the X component of the linear velocity of the RigidBody2D in world-units per second
		// linearVelocityY					:				the Y component of the linear velocity of the RigidBody2D in world-units per second
		// localToWorldMatrix				:				the transformation matrix used to transform the RigidBody2D to world space
		// mass								:				the mass of the RigidBody2D
		// position							:				the position of the RigidBody2D
		// rotation							:				the rotation of the RigidBody2D
		// sharedMaterial					:				the PhysicsMaterial2D that is applied to all Collider2D attached to this RigidBody2D
		// simulated						:				indicates whether the RigidBody2D should be simulated or not by the physics system
		// sleepMode						:				the sleep state that the RigidBody2D will initially be in
		// totalForce						:				the total amount of force that has been explicitly applied to this RigidBody2D since the last physics simulation step
		// totalTorque						:				the total amount of torque that has been explicitly applied to this RigidBody2D since the last physics simulation step
		// useFullKinematicContacts			:				should kinematic/kinematic and kinematic/static collisions be allowed?
		// worldCenterOfMass				:				the center of mass of the RigidBody2D in world space

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