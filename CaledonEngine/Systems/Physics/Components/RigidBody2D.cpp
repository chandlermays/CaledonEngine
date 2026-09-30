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
	constexpr float kMinMass = 0.0001f;				// Smallest allowed mass/inertia; keeps force / mass finite
}

namespace
{
	constexpr float kContactSkin = 0.001f;			// Penetration at or below this counts as touching, not blocked
	constexpr float kMinSubStep = 0.01f;			// Smallest sub-step length; guards degenerate (near-zero-size) colliders
	constexpr int kMaxLoopsPerIteration = 64;		// Bounds worst-case sub-stepping cost per allowed iteration
	constexpr float kSlideEpsilon = 1e-5f;			// Displacement below this is treated as zero

	/*------------------------------------------------------------------------------------------------
	| --- ScopedTransformPosition: Restores the Transform (and refreshes the collider) on scope exit --- |
	------------------------------------------------------------------------------------------------*/
	class ScopedTransformPosition
	{
	private:
		CE::Transform& m_transform;
		CE::Collider2D& m_collider;
		CE::Vector2f m_original;

	public:
		ScopedTransformPosition(CE::Transform& transform, CE::Collider2D& collider)
			: m_transform{ transform }
			, m_collider{ collider }
			, m_original{ transform.GetPosition() }
		{
		}

		~ScopedTransformPosition()
		{
			m_transform.SetPosition(m_original);
			m_collider.RefreshBounds();
		}

		ScopedTransformPosition(const ScopedTransformPosition&) = delete;
		ScopedTransformPosition& operator=(const ScopedTransformPosition&) = delete;
	};

	struct SlideContext
	{
		CE::CollisionManager& collisionManager;		// Source of contacts
		CE::Collider2D& collider;					// The collider being slid
		CE::Transform& transform;					// Temporarily moved to test candidate positions
		const CE::GameObject* pOwner;				// Colliders on this GameObject never block the slide
		float maxSubStep;							// Longest single move between overlap tests
		int maxIterations;							// Maximum number of contact resolutions
	};

	/*--------------------------------------------------------------------------------------------------------
	| --- SlidePass: Moves 'position' by 'remaining' in sub-steps. On the first blocking contact of a --- |
	| --- sub-step it depenetrates along the contact MTV, then removes the part of the remaining move --- |
	| --- pointing into the surface. If pUp is given (gravity pass), a contact whose surface angle    --- |
	| --- from 'up' is within slipAngleDegrees anchors the body instead of sliding. Returns the       --- |
	| --- number of contact resolutions used.                                                         --- |
	--------------------------------------------------------------------------------------------------------*/
	int SlidePass(const SlideContext& ctx, CE::Vector2f& position, CE::Vector2f& remaining,
		const CE::Vector2f* pUp, float slipAngleDegrees)
	{
		int iterations = 0;
		int loops = 0;
		const int maxLoops = kMaxLoopsPerIteration * (ctx.maxIterations + 1);

		while (iterations < ctx.maxIterations
			&& remaining.SqrMagnitude() > kSlideEpsilon * kSlideEpsilon
			&& loops++ < maxLoops)
		{
			const float length = remaining.Magnitude();
			const CE::Vector2f step = (length > ctx.maxSubStep) ? remaining * (ctx.maxSubStep / length) : remaining;
			const CE::Vector2f candidate = position + step;

			ctx.transform.SetPosition(candidate);
			ctx.collider.RefreshBounds();

			const std::vector<CE::Contact2D> contacts = ctx.collisionManager.QueryContacts(&ctx.collider);

			const CE::Contact2D* pDeepest = nullptr;
			for (const CE::Contact2D& contact : contacts)
			{
				if (contact.isTrigger || contact.depth <= kContactSkin)
					continue;

				if (contact.pColliderB->GetOwner() == ctx.pOwner)
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
/*-------------------------------------------------------------------
| --- Constructor: Constructs the RigidBody2D with default values --- |
-------------------------------------------------------------------*/
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
	, m_pSharedMaterial{ nullptr }
	, m_pCollisionManager{ nullptr }
{
}

/*-------------------------------------------------------
| --- Destructor: Unregisters from the PhysicsManager --- |
-------------------------------------------------------*/
CE::RigidBody2D::~RigidBody2D()
{
	PhysicsManager* pPhysicsManager = EngineManager::GetInstance().GetPhysicsManager();
	if (pPhysicsManager)
	{
		pPhysicsManager->RemoveBody(this);
	}
}

/*---------------------------------------------------------------------------------------
| --- Initialize: Registers with the PhysicsManager and applies the initial sleep mode --- |
---------------------------------------------------------------------------------------*/
bool CE::RigidBody2D::Initialize()
{
	m_pCollisionManager = EngineManager::GetInstance().GetCollisionManager();

	PhysicsManager* pPhysicsManager = EngineManager::GetInstance().GetPhysicsManager();
	if (!pPhysicsManager)
		return false;

	pPhysicsManager->AddBody(this);

	if (m_sleepMode == BodySleepMode2D::StartAsleep)
		Sleep();
	else
		WakeUp();

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

/*----------------------------------------------------------------------------------------------------------
| --- Step: Integrates forces, gravity, damping, and velocity into the Transform for one physics step --- |
----------------------------------------------------------------------------------------------------------*/
void CE::RigidBody2D::Step(float deltaTime, const Vector2f& gravity)
{
	if (!m_isSimulated || !m_pOwner || deltaTime <= 0.0f)
		return;

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

/*----------------------------------------------------
| --- GetBodyType / SetBodyType: The body's type --- |
----------------------------------------------------*/
CE::BodyType2D CE::RigidBody2D::GetBodyType() const { return m_bodyType; }

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

/*---------------------------------------------------------------
| --- Constraints: Which degrees of freedom are frozen --- |
---------------------------------------------------------------*/
CE::BodyConstraints2D CE::RigidBody2D::GetConstraints() const { return m_constraints; }

void CE::RigidBody2D::SetConstraints(BodyConstraints2D constraints)
{
	m_constraints = constraints;
	ApplyConstraintsToVelocity();
}

bool CE::RigidBody2D::GetFreezeRotation() const
{
	return HasFlag(m_constraints, BodyConstraints2D::FreezeRotation);
}

void CE::RigidBody2D::SetFreezeRotation(bool freeze)
{
	if (freeze)
		m_constraints |= BodyConstraints2D::FreezeRotation;
	else
		m_constraints &= ~BodyConstraints2D::FreezeRotation;

	ApplyConstraintsToVelocity();
}

/*------------------------------------------------
| --- Simple mode properties (get / set) --- |
------------------------------------------------*/
CE::BodyInterpolation2D CE::RigidBody2D::GetInterpolation() const { return m_interpolation; }
void CE::RigidBody2D::SetInterpolation(BodyInterpolation2D mode) { m_interpolation = mode; }

CE::BodySleepMode2D CE::RigidBody2D::GetSleepMode() const { return m_sleepMode; }
void CE::RigidBody2D::SetSleepMode(BodySleepMode2D mode) { m_sleepMode = mode; }

CE::CollisionDetectionMode2D CE::RigidBody2D::GetCollisionDetectionMode() const { return m_collisionDetectionMode; }
void CE::RigidBody2D::SetCollisionDetectionMode(CollisionDetectionMode2D mode) { m_collisionDetectionMode = mode; }

/*------------------------------------------------------------
| --- Velocity: Linear (whole / X / Y) and angular values --- |
------------------------------------------------------------*/
const CE::Vector2f& CE::RigidBody2D::GetLinearVelocity() const { return m_linearVelocity; }

void CE::RigidBody2D::SetLinearVelocity(const Vector2f& velocity)
{
	m_linearVelocity = velocity;
	ApplyConstraintsToVelocity();
}

float CE::RigidBody2D::GetLinearVelocityX() const { return m_linearVelocity.x; }

void CE::RigidBody2D::SetLinearVelocityX(float x)
{
	m_linearVelocity.x = x;
	ApplyConstraintsToVelocity();
}

float CE::RigidBody2D::GetLinearVelocityY() const { return m_linearVelocity.y; }

void CE::RigidBody2D::SetLinearVelocityY(float y)
{
	m_linearVelocity.y = y;
	ApplyConstraintsToVelocity();
}

float CE::RigidBody2D::GetAngularVelocity() const { return m_angularVelocity; }

void CE::RigidBody2D::SetAngularVelocity(float velocity)
{
	m_angularVelocity = velocity;
	ApplyConstraintsToVelocity();
}

/*--------------------------------------------------------------
| --- Damping, gravity scale, mass, inertia (get / set) --- |
--------------------------------------------------------------*/
float CE::RigidBody2D::GetLinearDamping() const { return m_linearDamping; }
void CE::RigidBody2D::SetLinearDamping(float damping) { m_linearDamping = std::max(damping, 0.0f); }

float CE::RigidBody2D::GetAngularDamping() const { return m_angularDamping; }
void CE::RigidBody2D::SetAngularDamping(float damping) { m_angularDamping = std::max(damping, 0.0f); }

float CE::RigidBody2D::GetGravityScale() const { return m_gravityScale; }
void CE::RigidBody2D::SetGravityScale(float scale) { m_gravityScale = scale; }

float CE::RigidBody2D::GetMass() const { return m_mass; }
void CE::RigidBody2D::SetMass(float mass) { m_mass = std::max(mass, kMinMass); }

float CE::RigidBody2D::GetInertia() const { return m_inertia; }
void CE::RigidBody2D::SetInertia(float inertia) { m_inertia = std::max(inertia, kMinMass); }

/*----------------------------------------------------------
| --- Center of mass (local and world) --- |
----------------------------------------------------------*/
const CE::Vector2f& CE::RigidBody2D::GetCenterOfMass() const { return m_centerOfMass; }
void CE::RigidBody2D::SetCenterOfMass(const Vector2f& centerOfMass) { m_centerOfMass = centerOfMass; }

CE::Vector2f CE::RigidBody2D::GetWorldCenterOfMass() const
{
	return GetRelativePoint(m_centerOfMass);
}

/*--------------------------------------------------------------------------
| --- Position / Rotation: Forwarded to the owner's Transform --- |
--------------------------------------------------------------------------*/
CE::Vector2f CE::RigidBody2D::GetPosition() const
{
	return m_pOwner ? m_pOwner->GetTransform().GetPosition() : Vector2f::Zero();
}

void CE::RigidBody2D::SetPosition(const Vector2f& position)
{
	if (m_pOwner)
		m_pOwner->GetTransform().SetPosition(position);
}

float CE::RigidBody2D::GetRotation() const
{
	return m_pOwner ? m_pOwner->GetTransform().GetRotation() : 0.0f;
}

void CE::RigidBody2D::SetRotation(float angle)
{
	if (m_pOwner)
		m_pOwner->GetTransform().SetRotation(angle);
}

/*------------------------------------------------------------------
| --- Flags, material, collider count (get / set) --- |
------------------------------------------------------------------*/
bool CE::RigidBody2D::IsSimulated() const { return m_isSimulated; }
void CE::RigidBody2D::SetSimulated(bool simulated) { m_isSimulated = simulated; }

bool CE::RigidBody2D::GetUseFullKinematicContacts() const { return m_useFullKinematicContacts; }
void CE::RigidBody2D::SetUseFullKinematicContacts(bool use) { m_useFullKinematicContacts = use; }

CE::PhysicsMaterial2D* CE::RigidBody2D::GetSharedMaterial() const { return m_pSharedMaterial; }
void CE::RigidBody2D::SetSharedMaterial(PhysicsMaterial2D* pMaterial) { m_pSharedMaterial = pMaterial; }

int CE::RigidBody2D::GetColliderCount() const
{
	return static_cast<int>(GetComponents<Collider2D>().size());
}

const CE::Vector2f& CE::RigidBody2D::GetTotalForce() const { return m_totalForce; }
float CE::RigidBody2D::GetTotalTorque() const { return m_totalTorque; }

/*-------------------------------------------------------------------------------
| --- AddForce: Applies a force (or instant impulse) in world space --- |
-------------------------------------------------------------------------------*/
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

void CE::RigidBody2D::AddForceX(float force, ForceMode2D mode) { AddForce(Vector2f(force, 0.0f), mode); }
void CE::RigidBody2D::AddForceY(float force, ForceMode2D mode) { AddForce(Vector2f(0.0f, force), mode); }

/*--------------------------------------------------------------------------------
| --- AddRelativeForce: Applies a force in the body's rotated (local) space --- |
--------------------------------------------------------------------------------*/
void CE::RigidBody2D::AddRelativeForce(const Vector2f& relativeForce, ForceMode2D mode)
{
	AddForce(GetRelativeVector(relativeForce), mode);
}

void CE::RigidBody2D::AddRelativeForceX(float force, ForceMode2D mode) { AddRelativeForce(Vector2f(force, 0.0f), mode); }
void CE::RigidBody2D::AddRelativeForceY(float force, ForceMode2D mode) { AddRelativeForce(Vector2f(0.0f, force), mode); }

/*------------------------------------------------------------------------------------
| --- AddForceAtPosition: Applies a force at a world position (adds torque too) --- |
------------------------------------------------------------------------------------*/
void CE::RigidBody2D::AddForceAtPosition(const Vector2f& force, const Vector2f& position, ForceMode2D mode)
{
	if (m_bodyType != BodyType2D::Dynamic || !m_isSimulated)
		return;

	AddForce(force, mode);

	// 2D torque = r x F, where r is the lever arm from the world center of mass
	const Vector2f leverArm = position - GetWorldCenterOfMass();
	AddTorque(Vector2f::Cross(leverArm, force), mode);
}

/*----------------------------------------------------------------------
| --- AddTorque: Applies a torque about the center of mass --- |
----------------------------------------------------------------------*/
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

/*-----------------------------------------------------------------------------------------------
| --- Space conversion helpers: rotation + translation only (Transform scale is not applied) --- |
-----------------------------------------------------------------------------------------------*/
CE::Vector2f CE::RigidBody2D::GetPoint(const Vector2f& worldPoint) const
{
	return (worldPoint - GetPosition()).Rotate(-GetRotation());
}

CE::Vector2f CE::RigidBody2D::GetRelativePoint(const Vector2f& localPoint) const
{
	return GetPosition() + localPoint.Rotate(GetRotation());
}

CE::Vector2f CE::RigidBody2D::GetVector(const Vector2f& worldVector) const
{
	return worldVector.Rotate(-GetRotation());
}

CE::Vector2f CE::RigidBody2D::GetRelativeVector(const Vector2f& localVector) const
{
	return localVector.Rotate(GetRotation());
}

/*--------------------------------------------------------------------------------
| --- GetPointVelocity: Velocity of the body at a point (v + w x r) --- |
--------------------------------------------------------------------------------*/
CE::Vector2f CE::RigidBody2D::GetPointVelocity(const Vector2f& worldPoint) const
{
	const Vector2f leverArm = worldPoint - GetWorldCenterOfMass();
	const float angularVelocityRadians = m_angularVelocity * Math::DEG2RAD;
	return m_linearVelocity + leverArm.Perpendicular() * angularVelocityRadians;
}

CE::Vector2f CE::RigidBody2D::GetRelativePointVelocity(const Vector2f& localPoint) const
{
	return GetPointVelocity(GetRelativePoint(localPoint));
}

/*-------------------------------------------------------------
| --- Sleep state: IsAwake / IsSleeping / Sleep / WakeUp --- |
-------------------------------------------------------------*/
bool CE::RigidBody2D::IsAwake() const { return !m_isSleeping; }
bool CE::RigidBody2D::IsSleeping() const { return m_isSleeping; }

void CE::RigidBody2D::Sleep()
{
	m_isSleeping = true;
	m_linearVelocity = Vector2f::Zero();
	m_angularVelocity = 0.0f;
	ClearAccumulators();
}

void CE::RigidBody2D::WakeUp()
{
	m_isSleeping = false;
}


/*----------------------------------------------------------------------------------------------------
| --- Slide: Calculates where the body ends up after sliding along surfaces (does not move it) --- |
----------------------------------------------------------------------------------------------------*/
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
	ScopedTransformPosition restoreTransform(transform, *pCollider);		// Candidate positions are tested by moving the Transform; restored on return

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

/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*----------------------------------------------------------------------------------
| --- ApplyConstraintsToVelocity: Zeroes velocity that the constraints freeze --- |
----------------------------------------------------------------------------------*/
void CE::RigidBody2D::ApplyConstraintsToVelocity()
{
	if (HasFlag(m_constraints, BodyConstraints2D::FreezePositionX))
		m_linearVelocity.x = 0.0f;

	if (HasFlag(m_constraints, BodyConstraints2D::FreezePositionY))
		m_linearVelocity.y = 0.0f;

	if (HasFlag(m_constraints, BodyConstraints2D::FreezeRotation))
		m_angularVelocity = 0.0f;
}

/*-----------------------------------------------------------------
| --- ClearAccumulators: Clears accumulated force and torque --- |
-----------------------------------------------------------------*/
void CE::RigidBody2D::ClearAccumulators()
{
	m_totalForce = Vector2f::Zero();
	m_totalTorque = 0.0f;
}