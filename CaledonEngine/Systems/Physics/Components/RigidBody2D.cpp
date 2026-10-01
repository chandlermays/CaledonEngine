/*------------------------------
| File: RigidBody2D.cpp
| Author: Chandler Mays
------------------------------*/
#include "RigidBody2D.h"

#include "Core/GameObject.h"
#include "Core/Transform.h"
#include "Systems/Engine/EngineManager.h"
#include "Systems/Physics/PhysicsManager.h"
#include "Systems/Physics/CollisionManager.h"
#include "Systems/Physics/Components/Collider2D.h"

#include <algorithm>
#include <cmath>
#include <vector>

namespace
{
	constexpr float kMinMass = 0.0001f;					// Smallest allowed mass/inertia; keeps force / mass finite
}

namespace
{
	constexpr float kContactSkin = 0.001f;				// Penetration at or below this counts as touching, not blocked
	constexpr float kMinSubStep = 0.01f;				// Smallest sub-step length; guards degenerate (near-zero-size) colliders
	constexpr int kMaxLoopsPerIteration = 64;			// Bounds worst-case sub-stepping cost per allowed iteration
	constexpr float kSlideEpsilon = 1e-5f;				// Displacement below this is treated as zero

	/*----------------------------------------------------------------------------------------------------
	| --- ScopedTransformPosition: Restores the Transform (and refreshes the collider) on scope exit --- |
	----------------------------------------------------------------------------------------------------*/
	class ScopedTransformPosition
	{
	private:
		CE::Transform& m_transform;
		std::vector<CE::Collider2D*> m_colliders;
		CE::Vector2f m_originalPosition;
		float m_originalRotation;

	public:
		ScopedTransformPosition(CE::Transform& transform, std::vector<CE::Collider2D*> colliders)
			: m_transform{ transform }
			, m_colliders{ std::move(colliders) }
			, m_originalPosition{ transform.GetPosition() }
			, m_originalRotation{ transform.GetRotation() }
		{ }

		~ScopedTransformPosition()
		{
			m_transform.SetPosition(m_originalPosition);
			m_transform.SetRotation(m_originalRotation);

			for (CE::Collider2D* pCollider : m_colliders)
			{
				pCollider->RefreshBounds();
			}
		}

		ScopedTransformPosition(const ScopedTransformPosition&) = delete;
		ScopedTransformPosition& operator=(const ScopedTransformPosition&) = delete;
	};

	struct SlideContext
	{
		CE::CollisionManager& collisionManager;			// Source of contacts
		CE::Collider2D& collider;						// The collider being slid
		CE::Transform& transform;						// Temporarily moved to test candidate positions
		const CE::GameObject* pOwner;					// Colliders on this GameObject never block the slide
		float maxSubStep;								// Longest single move between overlap tests
		int maxIterations;								// Maximum number of contact resolutions
	};

	/*---------------------------------------------------------------------------------------------------------------
	| --- SlidePass: Attempts to move the collider along the remaining vector, sliding along surfaces as needed --- |
	---------------------------------------------------------------------------------------------------------------*/
	int SlidePass(const SlideContext& context, CE::Vector2f& position, CE::Vector2f& remaining, const CE::Vector2f* pUp, float slipAngleDegrees)
	{
		int iterations = 0;
		int loops = 0;
		const int maxLoops = kMaxLoopsPerIteration * (context.maxIterations + 1);

		while (iterations < context.maxIterations && remaining.SqrMagnitude() > kSlideEpsilon * kSlideEpsilon && loops++ < maxLoops)
		{
			const float length = remaining.Magnitude();
			const CE::Vector2f step = (length > context.maxSubStep) ? remaining * (context.maxSubStep / length) : remaining;
			const CE::Vector2f candidate = position + step;

			context.transform.SetPosition(candidate);
			context.collider.RefreshBounds();

			const std::vector<CE::Contact2D> contacts = context.collisionManager.QueryContacts(&context.collider);

			const CE::Contact2D* pDeepest = nullptr;
			for (const CE::Contact2D& contact : contacts)
			{
				if (contact.isTrigger || contact.depth <= kContactSkin)
					continue;

				if (contact.pColliderB->GetOwner() == context.pOwner)
					continue;

				if (!pDeepest || contact.depth > pDeepest->depth)
				{
					pDeepest = &contact;
				}
			}

			remaining -= step;

			if (!pDeepest)
			{
				position = candidate;
				continue;
			}

			++iterations;

			// Normal points from the obstacle toward the slider, so this pushes the slider out of it
			const CE::Vector2f normal = pDeepest->normal;
			position = candidate + normal * pDeepest->depth;

			if (pUp)
			{
				const float cosAngle = std::clamp(CE::Vector2f::Dot(normal, *pUp), -1.0f, 1.0f);
				const float surfaceAngle = std::acos(cosAngle) * CE::Math::RAD2DEG;

				if (surfaceAngle <= slipAngleDegrees)
				{
					remaining = CE::Vector2f::Zero();		// Anchored: the surface is flat enough to stand on
					continue;
				}
			}

			// Slide: keep only the part of the remaining move that runs along the surface
			const float into = CE::Vector2f::Dot(remaining, normal);
			if (into < 0.0f)
			{
				remaining -= normal * into;
			}
		}

		return iterations;
	}
}

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*---------------------------------------------------------------------
| --- Constructor: Constructs the RigidBody2D with default values --- |
---------------------------------------------------------------------*/
CE::RigidBody2D::RigidBody2D()
	: Component()
	, m_bodyType{ BodyType2D::Dynamic }
	, m_constraints{ BodyConstraints2D::None }
	, m_interpolation{ BodyInterpolation2D::None }
	, m_sleepMode{ BodySleepMode2D::StartAwake }
	, m_collisionDetectionMode{ CollisionDetectionMode2D::Discrete }
	, m_linearVelocity{ 0.0f, 0.0f }
	, m_angularVelocity{ 0.0f }
	, m_linearDamping{ 0.0f }
	, m_angularDamping{ 0.05f }
	, m_gravityScale{ 1.0f }
	, m_mass{ 1.0f }
	, m_inertia{ 1.0f }
	, m_centerOfMass{ 0.0f, 0.0f }
	, m_totalForce{ 0.0f, 0.0f }
	, m_totalTorque{ 0.0f }
	, m_isSimulated{ true }
	, m_useFullKinematicContacts{ false }
	, m_isSleeping{ false }
	, m_hasPendingPosition{ false }
	, m_pendingPosition{ 0.0f, 0.0f }
	, m_hasPendingRotation{ false }
	, m_pendingRotation{ 0.0f }
	, m_pSharedMaterial{ nullptr }
	, m_pCollisionManager{ nullptr }
{ }

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
CE::RigidBody2D::~RigidBody2D()
{
	PhysicsManager* pPhysicsManager = EngineManager::GetInstance().GetPhysicsManager();
	if (pPhysicsManager)
	{
		pPhysicsManager->RemoveBody(this);
	}
}

/*------------------------------------------------------
| --- Initialize: Prepares the RigidBody2D for use --- |
------------------------------------------------------*/
bool CE::RigidBody2D::Initialize()
{
	m_pCollisionManager = EngineManager::GetInstance().GetCollisionManager();

	PhysicsManager* pPhysicsManager = EngineManager::GetInstance().GetPhysicsManager();
	if (!pPhysicsManager)
		return false;

	pPhysicsManager->AddBody(this);

	if (m_sleepMode == BodySleepMode2D::StartAsleep)
	{
		Sleep();
	}
	else
	{
		WakeUp();
	}

	return true;
}

/*-------------------------------------------------------------
| --- GetTypeName: Returns the type name of the Component --- |
-------------------------------------------------------------*/
const std::string& CE::RigidBody2D::GetTypeName() const
{
	static const std::string typeName = "RigidBody2D";
	return typeName;
}

/*---------------------------------------------------------------------------------------------------------
| --- Step: Integrates forces, gravity, damping, and velocity into the Transform for one physics step --- |
---------------------------------------------------------------------------------------------------------*/
void CE::RigidBody2D::Step(float deltaTime, const Vector2f& gravity)
{
	if (!m_isSimulated || !m_pOwner || deltaTime <= 0.0f)
		return;

	ApplyPendingMoves();

	if (m_isSleeping)
		return;

	if (m_bodyType == BodyType2D::Static)
	{
		m_linearVelocity = Vector2f::Zero();
		m_angularVelocity = 0.0f;
		ClearAccumulators();
		return;
	}

	// Only Dynamic bodies respond to forces, gravity, and damping; Kinematic bodies move purely by their velocity.
	if (m_bodyType == BodyType2D::Dynamic)
	{
		Vector2f acceleration = m_totalForce / m_mass + gravity * m_gravityScale;
		m_linearVelocity += acceleration * deltaTime;
		m_angularVelocity += (m_totalTorque / m_inertia) * deltaTime;

		// Implicit damping (Box2D-style): stable for any deltaTime and any damping >= 0
		m_linearVelocity /= (1.0f + deltaTime * m_linearDamping);
		m_angularVelocity /= (1.0f + deltaTime * m_angularDamping);
	}

	ApplyConstraintsToVelocity();

	Transform& transform = m_pOwner->GetTransform();
	transform.SetPosition(transform.GetPosition() + m_linearVelocity * deltaTime);
	transform.SetRotation(transform.GetRotation() + m_angularVelocity * deltaTime);

	ClearAccumulators();
}

/*-----------------------------------------------------------------------------
| --- GetBodyType: Returns the physical behavior type of this RigidBody2D --- |
-----------------------------------------------------------------------------*/
CE::BodyType2D CE::RigidBody2D::GetBodyType() const 
{ 
	return m_bodyType;
}

/*--------------------------------------------------------------------------
| --- SetBodyType: Sets the physical behavior type of this RigidBody2D --- |
--------------------------------------------------------------------------*/
void CE::RigidBody2D::SetBodyType(BodyType2D type)
{
	m_bodyType = type;
	WakeUp();

	if (m_bodyType == BodyType2D::Static)
	{
		m_linearVelocity = Vector2f::Zero();
		m_angularVelocity = 0.0f;
		ClearAccumulators();
	}
}

/*-------------------------------------------------------------------------------------
| --- GetConstraints: Returns the degrees of freedom allowed for this RigidBody2D --- |
-------------------------------------------------------------------------------------*/
CE::BodyConstraints2D CE::RigidBody2D::GetConstraints() const
{
	return m_constraints;
}

/*----------------------------------------------------------------------------------
| --- SetConstraints: Sets the degrees of freedom allowed for this RigidBody2D --- |
----------------------------------------------------------------------------------*/
void CE::RigidBody2D::SetConstraints(BodyConstraints2D constraints)
{
	m_constraints = constraints;
	ApplyConstraintsToVelocity();
}

/*------------------------------------------------------------------------------------
| --- GetFreezeRotation: Returns whether rotation is frozen for this RigidBody2D --- |
------------------------------------------------------------------------------------*/
bool CE::RigidBody2D::GetFreezeRotation() const
{
	return HasFlag(m_constraints, BodyConstraints2D::FreezeRotation);
}

/*---------------------------------------------------------------------------------
| --- SetFreezeRotation: Sets whether rotation is frozen for this RigidBody2D --- |
---------------------------------------------------------------------------------*/
void CE::RigidBody2D::SetFreezeRotation(bool freeze)
{
	if (freeze)
	{
		m_constraints |= BodyConstraints2D::FreezeRotation;
	}
	else
	{
		m_constraints &= ~BodyConstraints2D::FreezeRotation;
	}

	ApplyConstraintsToVelocity();
}

/*---------------------------------------------------------------------------------------
| --- GetInterpolation: Returns the physics interpolation mode for this RigidBody2D --- |
---------------------------------------------------------------------------------------*/
CE::BodyInterpolation2D CE::RigidBody2D::GetInterpolation() const
{
	return m_interpolation;
}

/*------------------------------------------------------------------------------------
| --- SetInterpolation: Sets the physics interpolation mode for this RigidBody2D --- |
------------------------------------------------------------------------------------*/
void CE::RigidBody2D::SetInterpolation(BodyInterpolation2D mode)
{
	m_interpolation = mode;
}

/*-------------------------------------------------------------------
| --- GetSleepMode: Returns the sleep mode for this RigidBody2D --- |
-------------------------------------------------------------------*/
CE::BodySleepMode2D CE::RigidBody2D::GetSleepMode() const
{
	return m_sleepMode;
}

/*----------------------------------------------------------------
| --- SetSleepMode: Sets the sleep mode for this RigidBody2D --- |
----------------------------------------------------------------*/
void CE::RigidBody2D::SetSleepMode(BodySleepMode2D mode)
{
	m_sleepMode = mode;
}

/*----------------------------------------------------------------------------------------------
| --- GetCollisionDetectionMode: Returns the collision detection mode for this RigidBody2D --- |
----------------------------------------------------------------------------------------------*/
CE::CollisionDetectionMode2D CE::RigidBody2D::GetCollisionDetectionMode() const
{
	return m_collisionDetectionMode;
}

/*-------------------------------------------------------------------------------------------
| --- SetCollisionDetectionMode: Sets the collision detection mode for this RigidBody2D --- |
-------------------------------------------------------------------------------------------*/
void CE::RigidBody2D::SetCollisionDetectionMode(CollisionDetectionMode2D mode)
{ 
	m_collisionDetectionMode = mode;
}

/*----------------------------------------------------------------------------
| --- GetLinearVelocity: Returns the linear velocity of this RigidBody2D --- |
----------------------------------------------------------------------------*/
const CE::Vector2f& CE::RigidBody2D::GetLinearVelocity() const
{
	return m_linearVelocity;
}

/*-------------------------------------------------------------------------
| --- SetLinearVelocity: Sets the linear velocity of this RigidBody2D --- |
-------------------------------------------------------------------------*/
void CE::RigidBody2D::SetLinearVelocity(const Vector2f& velocity)
{
	m_linearVelocity = velocity;
	ApplyConstraintsToVelocity();
}

/*-----------------------------------------------------------------------------------------
| --- GetLinearVelocityX: Returns the linear velocity X component of this RigidBody2D --- |
-----------------------------------------------------------------------------------------*/
float CE::RigidBody2D::GetLinearVelocityX() const
{
	return m_linearVelocity.x;
}

/*--------------------------------------------------------------------------------------
| --- SetLinearVelocityX: Sets the linear velocity X component of this RigidBody2D --- |
--------------------------------------------------------------------------------------*/
void CE::RigidBody2D::SetLinearVelocityX(float x)
{
	m_linearVelocity.x = x;
	ApplyConstraintsToVelocity();
}

/*-----------------------------------------------------------------------------------------
| --- GetLinearVelocityY: Returns the linear velocity Y component of this RigidBody2D --- |
-----------------------------------------------------------------------------------------*/
float CE::RigidBody2D::GetLinearVelocityY() const
{
	return m_linearVelocity.y;
}

/*--------------------------------------------------------------------------------------
| --- SetLinearVelocityY: Sets the linear velocity Y component of this RigidBody2D --- |
--------------------------------------------------------------------------------------*/
void CE::RigidBody2D::SetLinearVelocityY(float y)
{
	m_linearVelocity.y = y;
	ApplyConstraintsToVelocity();
}

/*------------------------------------------------------------------------------
| --- GetAngularVelocity: Returns the angular velocity of this RigidBody2D --- |
------------------------------------------------------------------------------*/
float CE::RigidBody2D::GetAngularVelocity() const
{
	return m_angularVelocity;
}

/*---------------------------------------------------------------------------
| --- SetAngularVelocity: Sets the angular velocity of this RigidBody2D --- |
---------------------------------------------------------------------------*/
void CE::RigidBody2D::SetAngularVelocity(float velocity)
{
	m_angularVelocity = velocity;
	ApplyConstraintsToVelocity();
}

/*--------------------------------------------------------------------------
| --- GetLinearDamping: Returns the linear damping of this RigidBody2D --- |
--------------------------------------------------------------------------*/
float CE::RigidBody2D::GetLinearDamping() const
{ 
	return m_linearDamping;
}

/*-----------------------------------------------------------------------
| --- SetLinearDamping: Sets the linear damping of this RigidBody2D --- |
-----------------------------------------------------------------------*/
void CE::RigidBody2D::SetLinearDamping(float damping)
{
	m_linearDamping = std::max(damping, 0.0f);
}

/*----------------------------------------------------------------------------
| --- GetAngularDamping: Returns the angular damping of this RigidBody2D --- |
----------------------------------------------------------------------------*/
float CE::RigidBody2D::GetAngularDamping() const
{
	return m_angularDamping;
}

/*-------------------------------------------------------------------------
| --- SetAngularDamping: Sets the angular damping of this RigidBody2D --- |
-------------------------------------------------------------------------*/
void CE::RigidBody2D::SetAngularDamping(float damping)
{
	m_angularDamping = std::max(damping, 0.0f);
}

/*------------------------------------------------------------------------
| --- GetGravityScale: Returns the gravity scale of this RigidBody2D --- |
------------------------------------------------------------------------*/
float CE::RigidBody2D::GetGravityScale() const
{
	return m_gravityScale;
}

/*---------------------------------------------------------------------
| --- SetGravityScale: Sets the gravity scale of this RigidBody2D --- |
---------------------------------------------------------------------*/
void CE::RigidBody2D::SetGravityScale(float scale)
{
	m_gravityScale = scale;
}

/*-------------------------------------------------------
| --- GetMass: Returns the mass of this RigidBody2D --- |
-------------------------------------------------------*/
float CE::RigidBody2D::GetMass() const
{
	return m_mass;
}

/*----------------------------------------------------
| --- SetMass: Sets the mass of this RigidBody2D --- |
----------------------------------------------------*/
void CE::RigidBody2D::SetMass(float mass)
{
	m_mass = std::max(mass, kMinMass);
}

/*-------------------------------------------------------------
| --- GetInertia: Returns the inertia of this RigidBody2D --- |
-------------------------------------------------------------*/
float CE::RigidBody2D::GetInertia() const
{
	return m_inertia;
}

/*----------------------------------------------------------
| --- SetInertia: Sets the inertia of this RigidBody2D --- |
----------------------------------------------------------*/
void CE::RigidBody2D::SetInertia(float inertia)
{ 
	m_inertia = std::max(inertia, kMinMass);
}

/*----------------------------------------------------------------------------------------
| --- GetCenterOfMass: Returns the center of mass of this RigidBody2D in local space --- |
----------------------------------------------------------------------------------------*/
const CE::Vector2f& CE::RigidBody2D::GetCenterOfMass() const
{
	return m_centerOfMass;
}

/*-------------------------------------------------------------------------------------
| --- SetCenterOfMass: Sets the center of mass of this RigidBody2D in local space --- |
-------------------------------------------------------------------------------------*/
void CE::RigidBody2D::SetCenterOfMass(const Vector2f& centerOfMass)
{
	m_centerOfMass = centerOfMass;
}

/*---------------------------------------------------------------------------------------------
| --- GetWorldCenterOfMass: Returns the center of mass of this RigidBody2D in world space --- |
---------------------------------------------------------------------------------------------*/
CE::Vector2f CE::RigidBody2D::GetWorldCenterOfMass() const
{
	return GetRelativePoint(m_centerOfMass);
}

/*------------------------------------------------------------------------------
| --- GetPosition: Returns the position of this RigidBody2D in world space --- |
------------------------------------------------------------------------------*/
CE::Vector2f CE::RigidBody2D::GetPosition() const
{
	return m_pOwner ? m_pOwner->GetTransform().GetPosition() : Vector2f::Zero();
}

/*---------------------------------------------------------------------------
| --- SetPosition: Sets the position of this RigidBody2D in world space --- |
---------------------------------------------------------------------------*/
void CE::RigidBody2D::SetPosition(const Vector2f& position)
{
	if (m_pOwner)
	{
		m_pOwner->GetTransform().SetPosition(position);
	}
}

/*------------------------------------------------------------------------------
| --- GetRotation: Returns the rotation of this RigidBody2D in world space --- |
------------------------------------------------------------------------------*/
float CE::RigidBody2D::GetRotation() const
{
	return m_pOwner ? m_pOwner->GetTransform().GetRotation() : 0.0f;
}

/*---------------------------------------------------------------------------
| --- SetRotation: Sets the rotation of this RigidBody2D in world space --- |
---------------------------------------------------------------------------*/
void CE::RigidBody2D::SetRotation(float angle)
{
	if (m_pOwner)
	{
		m_pOwner->GetTransform().SetRotation(angle);
	}
}

/*---------------------------------------------------------------------------
| --- IsSimulated: Returns whether this RigidBody2D is simulated or not --- |
---------------------------------------------------------------------------*/
bool CE::RigidBody2D::IsSimulated() const
{
	return m_isSimulated;
}

/*-------------------------------------------------------------------------
| --- SetSimulated: Sets whether this RigidBody2D is simulated or not --- |
-------------------------------------------------------------------------*/
void CE::RigidBody2D::SetSimulated(bool simulated)
{
	m_isSimulated = simulated;
}

/*---------------------------------------------------------------------------------------------------------------------------
| --- GetUseFullKinematicContacts: Returns whether kinematic/kinematic and kinematic/static contacts are allowed or not --- |
---------------------------------------------------------------------------------------------------------------------------*/
bool CE::RigidBody2D::GetUseFullKinematicContacts() const
{
	return m_useFullKinematicContacts;
}

/*------------------------------------------------------------------------------------------------------------------------
| --- SetUseFullKinematicContacts: Sets whether kinematic/kinematic and kinematic/static contacts are allowed or not --- |
------------------------------------------------------------------------------------------------------------------------*/
void CE::RigidBody2D::SetUseFullKinematicContacts(bool use)
{
	m_useFullKinematicContacts = use;
}

/*------------------------------------------------------------------------------------------------------
| --- GetSharedMaterial: Returns the PhysicsMaterial2D shared by all colliders on this RigidBody2D --- |
------------------------------------------------------------------------------------------------------*/
CE::PhysicsMaterial2D* CE::RigidBody2D::GetSharedMaterial() const
{
	return m_pSharedMaterial;
}

/*---------------------------------------------------------------------------------------------------
| --- SetSharedMaterial: Sets the PhysicsMaterial2D shared by all colliders on this RigidBody2D --- |
---------------------------------------------------------------------------------------------------*/
void CE::RigidBody2D::SetSharedMaterial(PhysicsMaterial2D* pMaterial)
{
	m_pSharedMaterial = pMaterial;
}

/*-----------------------------------------------------------------------------------
| --- GetColliderCount: Returns the number of Collider2D on the same GameObject --- |
-----------------------------------------------------------------------------------*/
int CE::RigidBody2D::GetColliderCount() const
{
	return static_cast<int>(GetComponents<Collider2D>().size());
}

/*------------------------------------------------------------------------------------------------------------
| --- GetTotalForce: Returns the total force applied to this RigidBody2D during the current physics step --- |
------------------------------------------------------------------------------------------------------------*/
const CE::Vector2f& CE::RigidBody2D::GetTotalForce() const
{
	return m_totalForce;
}

/*--------------------------------------------------------------------------------------------------------------
| --- GetTotalTorque: Returns the total torque applied to this RigidBody2D during the current physics step --- |
--------------------------------------------------------------------------------------------------------------*/
float CE::RigidBody2D::GetTotalTorque() const
{
	return m_totalTorque;
}

/*-------------------------------------------------------------------
| --- AddForce: Applies a world-space force to this RigidBody2D --- |
-------------------------------------------------------------------*/
void CE::RigidBody2D::AddForce(const Vector2f& force, ForceMode2D mode)
{
	if (m_bodyType != BodyType2D::Dynamic || !m_isSimulated)
		return;

	WakeUp();

	if (mode == ForceMode2D::Impulse)
	{
		m_linearVelocity += force / m_mass;
		ApplyConstraintsToVelocity();
	}
	else
	{
		m_totalForce += force;
	}
}

/*-------------------------------------------------------------------------------------
| --- AddForceX: Applies a world-space force to this RigidBody2D along the X-axis --- |
-------------------------------------------------------------------------------------*/
void CE::RigidBody2D::AddForceX(float force, ForceMode2D mode)
{ 
	AddForce(Vector2f(force, 0.0f), mode);
}

/*-------------------------------------------------------------------------------------
| --- AddForceY: Applies a world-space force to this RigidBody2D along the Y-axis --- |
-------------------------------------------------------------------------------------*/
void CE::RigidBody2D::AddForceY(float force, ForceMode2D mode)
{ 
	AddForce(Vector2f(0.0f, force), mode);
}

/*---------------------------------------------------------------------------
| --- AddRelativeForce: Applies a local-space force to this RigidBody2D --- |
---------------------------------------------------------------------------*/
void CE::RigidBody2D::AddRelativeForce(const Vector2f& relativeForce, ForceMode2D mode)
{
	AddForce(GetRelativeVector(relativeForce), mode);
}

/*---------------------------------------------------------------------------------------------
| --- AddRelativeForceX: Applies a local-space force to this RigidBody2D along the X-axis --- |
---------------------------------------------------------------------------------------------*/
void CE::RigidBody2D::AddRelativeForceX(float force, ForceMode2D mode)
{
	AddRelativeForce(Vector2f(force, 0.0f), mode);
}

/*---------------------------------------------------------------------------------------------
| --- AddRelativeForceY: Applies a local-space force to this RigidBody2D along the Y-axis --- |
---------------------------------------------------------------------------------------------*/
void CE::RigidBody2D::AddRelativeForceY(float force, ForceMode2D mode)
{
	AddRelativeForce(Vector2f(0.0f, force), mode);
}

/*-------------------------------------------------------------------------------------
| --- AddForceAtPosition: Applies a force to this RigidBody2D at a world position --- |
-------------------------------------------------------------------------------------*/
void CE::RigidBody2D::AddForceAtPosition(const Vector2f& force, const Vector2f& position, ForceMode2D mode)
{
	if (m_bodyType != BodyType2D::Dynamic || !m_isSimulated)
		return;

	AddForce(force, mode);

	// 2D torque = r x F, where r is the lever arm from the world center of mass
	const Vector2f leverArm = position - GetWorldCenterOfMass();
	AddTorque(Vector2f::Cross(leverArm, force), mode);
}

/*-----------------------------------------------------------------------------------
| --- AddTorque: Applies a torque to this RigidBody2D around its center of mass --- |
-----------------------------------------------------------------------------------*/
void CE::RigidBody2D::AddTorque(float torque, ForceMode2D mode)
{
	if (m_bodyType != BodyType2D::Dynamic || !m_isSimulated)
		return;

	WakeUp();

	if (mode == ForceMode2D::Impulse)
	{
		m_angularVelocity += torque / m_inertia;
		ApplyConstraintsToVelocity();
	}
	else
	{
		m_totalTorque += torque;
	}
}

/*-----------------------------------------------------------------------------------------------------
| --- GetPoint: Returns a world-space point converted to local-space relative to this RigidBody2D --- |
-----------------------------------------------------------------------------------------------------*/
CE::Vector2f CE::RigidBody2D::GetPoint(const Vector2f& worldPoint) const
{
	return (worldPoint - GetPosition()).Rotate(-GetRotation());
}

/*-------------------------------------------------------------------------------------------------------------
| --- GetRelativePoint: Returns a local-space point converted to world-space relative to this RigidBody2D --- |
-------------------------------------------------------------------------------------------------------------*/
CE::Vector2f CE::RigidBody2D::GetRelativePoint(const Vector2f& localPoint) const
{
	return GetPosition() + localPoint.Rotate(GetRotation());
}

/*-------------------------------------------------------------------------------------------------------
| --- GetVector: Returns a world-space vector converted to local-space relative to this RigidBody2D --- |
-------------------------------------------------------------------------------------------------------*/
CE::Vector2f CE::RigidBody2D::GetVector(const Vector2f& worldVector) const
{
	return worldVector.Rotate(-GetRotation());
}

/*---------------------------------------------------------------------------------------------------------------
| --- GetRelativeVector: Returns a local-space vector converted to world-space relative to this RigidBody2D --- |
---------------------------------------------------------------------------------------------------------------*/
CE::Vector2f CE::RigidBody2D::GetRelativeVector(const Vector2f& localVector) const
{
	return localVector.Rotate(GetRotation());
}

/*----------------------------------------------------------------------------------------------
| --- GetPointVelocity: Returns the velocity of a point in world space on this RigidBody2D --- |
----------------------------------------------------------------------------------------------*/
CE::Vector2f CE::RigidBody2D::GetPointVelocity(const Vector2f& worldPoint) const
{
	const Vector2f leverArm = worldPoint - GetWorldCenterOfMass();
	const float angularVelocityRadians = m_angularVelocity * Math::DEG2RAD;

	return m_linearVelocity + leverArm.Perpendicular() * angularVelocityRadians;
}

/*------------------------------------------------------------------------------------------------------
| --- GetRelativePointVelocity: Returns the velocity of a point in local space on this RigidBody2D --- |
------------------------------------------------------------------------------------------------------*/
CE::Vector2f CE::RigidBody2D::GetRelativePointVelocity(const Vector2f& localPoint) const
{
	return GetPointVelocity(GetRelativePoint(localPoint));
}

/*----------------------------------------------------------------------------------------------------------------
| --- Slide: Slide this RigidBody2D using the specified velocity and configuration integrated over deltaTime --- |
----------------------------------------------------------------------------------------------------------------*/
CE::SlideResults2D CE::RigidBody2D::Slide(const Vector2f& velocity, float deltaTime, const SlideConfig2D& config)
{
	SlideResults2D results;
	results.position = config.startPosition;

	if (!m_pOwner || deltaTime <= 0.0f)
		return results;

	Vector2f remaining = velocity * deltaTime;

	Collider2D* pCollider = config.selectedCollider ? config.selectedCollider : GetComponent<Collider2D>();
	CollisionManager* pCollisionManager = m_pCollisionManager;

	// Nothing to collide with: the whole move is free
	if (!pCollider || !pCollisionManager)
	{
		results.position = config.startPosition + remaining;
		return results;
	}

	Transform& transform = m_pOwner->GetTransform();
	ScopedTransformPosition restoreTransform(transform, std::vector<Collider2D*>{ pCollider });		// Candidate positions are tested by moving the Transform; restored on return

	pCollisionManager->RefreshAllBounds();

	// Sub-step no longer than the collider's smallest half-extent so a step can never jump over a thinner obstacle
	const Vector2f extents = pCollider->GetBounds().GetExtents();
	const SlideContext context
	{
		*pCollisionManager,
		*pCollider,
		transform,
		m_pOwner,
		std::max(std::min(extents.x, extents.y), kMinSubStep),
		std::max(config.maxIterations, 1)
	};

	Vector2f position = config.startPosition;

	// Pass 1: the requested movement, sliding along whatever blocks it
	results.iterationsUsed += SlidePass(context, position, remaining, nullptr, 0.0f);
	results.remainingVelocity = remaining / deltaTime;

	// Pass 2: gravity, applied as a displacement per second. Flat-enough surfaces anchor; steeper ones let the body slip.
	if (config.gravity.SqrMagnitude() > kSlideEpsilon * kSlideEpsilon)
	{
		Vector2f gravityDisplacement = config.gravity * deltaTime;
		const Vector2f up = -config.gravity.Normalized();
		results.iterationsUsed += SlidePass(context, position, gravityDisplacement, &up, config.gravitySlipAngle);
	}

	results.position = position;
	return results;
}

/*------------------------------------------------------------------------
| --- MovePosition: Moves this RigidBody2D to the specified position --- |
------------------------------------------------------------------------*/
void CE::RigidBody2D::MovePosition(const Vector2f& position)
{
	m_pendingPosition = position;
	m_hasPendingPosition = true;
	WakeUp();
}

/*-----------------------------------------------------------------------
| --- MoveRotation: Rotates this RigidBody2D to the specified angle --- |
-----------------------------------------------------------------------*/
void CE::RigidBody2D::MoveRotation(float angle)
{
	m_pendingRotation = angle;
	m_hasPendingRotation = true;
	WakeUp();
}

/*------------------------------------------------------------------------------------------------------
| --- MovePositionAndRotation: Moves this RigidBody2D to the specified position and rotation angle --- |
------------------------------------------------------------------------------------------------------*/
void CE::RigidBody2D::MovePositionAndRotation(const Vector2f & position, float angle)
{
	MovePosition(position);
	MoveRotation(angle);
}

/*---------------------------------------------------------------------------------------------------------------------------------------
| --- ClosestPoint: Returns a point on the perimeter of all enabled colliders on this RigidBody2D closest to the specified position --- |
---------------------------------------------------------------------------------------------------------------------------------------*/
CE::Vector2f CE::RigidBody2D::ClosestPoint(const Vector2f & position) const
{
	Vector2f closest = position;
	float closestSqrDistance = std::numeric_limits<float>::max();

	for (Collider2D* pCollider : GetComponents<Collider2D>())
	{
		if (!pCollider->IsActive())
			continue;

		const Vector2f point = pCollider->ClosestPoint(position);
		const float sqrDistance = Vector2f::SqrDistance(position, point);

		if (sqrDistance < closestSqrDistance)
		{
			closestSqrDistance = sqrDistance;
			closest = point;
		}
	}

	return closest;
}

/*------------------------------------------------------------------------------------------------------------------------
| --- Distance: Calculates the minimum distance of this collider against all Collider2D attached to this RigidBody2D --- |
------------------------------------------------------------------------------------------------------------------------*/
CE::ColliderDistance2D CE::RigidBody2D::Distance(const Collider2D& collider) const
{
	ColliderDistance2D best;

	for (Collider2D* pCollider : GetComponents<Collider2D>())
	{
		if (!pCollider->IsActive() || pCollider == &collider)
			continue;

		const ColliderDistance2D candidate = pCollider->Distance(collider);

		if (candidate.isValid && (!best.isValid || candidate.distance < best.distance))
		{
			best = candidate;
		}
	}

	return best;
}

/*--------------------------------------------------------------------------------------------------
| --- IsTouching: Returns whether this RigidBody2D is touching the specified Collider2D or not --- |
--------------------------------------------------------------------------------------------------*/
bool CE::RigidBody2D::IsTouching(const Collider2D& collider) const
{
	if (collider.IsTrigger())
		return false;

	for (Collider2D* pCollider : GetComponents<Collider2D>())
	{
		if (!pCollider->IsActive() || pCollider->IsTrigger() || pCollider == &collider)
			continue;

		if (pCollider->Overlaps(collider))
			return true;
	}

	return false;
}

/*----------------------------------------------------------------------------------------
| --- OverlapPoint: Checks if any of the attached colliders overlap a point in space --- |
----------------------------------------------------------------------------------------*/
bool CE::RigidBody2D::OverlapPoint(const Vector2f& point) const
{
	for (Collider2D* pCollider : GetComponents<Collider2D>())
	{
		if (pCollider->IsActive() && pCollider->OverlapPoint(point))
			return true;
	}

	return false;
}

/*----------------------------------------------------------------------------------------------------------
| --- Overlap: Returns a list of all colliders that overlap all attached colliders of this RigidBody2D --- |
----------------------------------------------------------------------------------------------------------*/
int CE::RigidBody2D::Overlap(const Vector2f& position, float angle, std::vector<Collider2D*>& results) const
{
	results.clear();

	if (!m_pOwner || !m_pCollisionManager)
		return 0;

	std::vector<Collider2D*> attached = GetComponents<Collider2D>();
	if (attached.empty())
		return 0;

	Transform& transform = m_pOwner->GetTransform();
	ScopedTransformPosition restoreTransform(transform, attached);		// Candidate positions are tested by moving the Transform; restored on return

	m_pCollisionManager->RefreshAllBounds();

	transform.SetPosition(position);
	transform.SetRotation(angle);

	for (Collider2D* pCollider : attached)
	{
		pCollider->RefreshBounds();
	}

	for (Collider2D* pCollider : attached)
	{
		if (!pCollider->IsActive())
			continue;

		for (const Contact2D& contact : m_pCollisionManager->QueryContacts(pCollider))
		{
			Collider2D* pOther = contact.pColliderB;

			if (pOther->GetOwner() == m_pOwner)
				continue;

			if (std::find(results.begin(), results.end(), pOther) == results.end())
			{
				results.push_back(pOther);
			}
		}
	}

	return static_cast<int>(results.size());
}

/*-------------------------------------------------------------------
| --- IsAwake: Returns whether this RigidBody2D is awake or not --- |
-------------------------------------------------------------------*/
bool CE::RigidBody2D::IsAwake() const
{
	return !m_isSleeping;
}

/*-------------------------------------------------------------------------
| --- IsSleeping: Returns whether this RigidBody2D is sleeping or not --- |
-------------------------------------------------------------------------*/
bool CE::RigidBody2D::IsSleeping() const
{
	return m_isSleeping;
}

/*-------------------------------------------------------------------------------
| --- Sleep: Puts this RigidBody2D to sleep, stopping all motion and forces --- |
-------------------------------------------------------------------------------*/
void CE::RigidBody2D::Sleep()
{
	m_isSleeping = true;
	m_linearVelocity = Vector2f::Zero();
	m_angularVelocity = 0.0f;
	ClearAccumulators();
}

/*------------------------------------------------------------------------------
| --- WakeUp: Wakes this RigidBody2D up, allowing it to be simulated again --- |
------------------------------------------------------------------------------*/
void CE::RigidBody2D::WakeUp()
{
	m_isSleeping = false;
}



/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------
| --- ApplyConstraintsToVelocity: Applies the constraints to the linear and angular velocity of this RigidBody2D --- |
--------------------------------------------------------------------------------------------------------------------*/
void CE::RigidBody2D::ApplyConstraintsToVelocity()
{
	if (HasFlag(m_constraints, BodyConstraints2D::FreezePositionX))
		m_linearVelocity.x = 0.0f;

	if (HasFlag(m_constraints, BodyConstraints2D::FreezePositionY))
		m_linearVelocity.y = 0.0f;

	if (HasFlag(m_constraints, BodyConstraints2D::FreezeRotation))
		m_angularVelocity = 0.0f;
}

/*----------------------------------------------------------------------------------------------------------------------------
| --- ClearAccumulators: Clears the accumulated force and torque applied to this RigidBody2D since the last physics step --- |
----------------------------------------------------------------------------------------------------------------------------*/
void CE::RigidBody2D::ClearAccumulators()
{
	m_totalForce = Vector2f::Zero();
	m_totalTorque = 0.0f;
}

/*-------------------------------------------------------------------------------------------------------------------------
| --- ApplyPendingMoves: Applies any pending position and rotation changes to this RigidBody2D after the physics step --- |
-------------------------------------------------------------------------------------------------------------------------*/
void CE::RigidBody2D::ApplyPendingMoves()
{
	if (!m_pOwner)
		return;

	if (m_hasPendingPosition)
	{
		SetPosition(m_pendingPosition);
		m_hasPendingPosition = false;
	}

	if (m_hasPendingRotation)
	{
		SetRotation(m_pendingRotation);
		m_hasPendingRotation = false;
	}
}