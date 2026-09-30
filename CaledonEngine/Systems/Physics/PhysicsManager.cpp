/*------------------------------
| File: PhysicsManager.cpp
| Author: Chandler Mays
------------------------------*/
#include "PhysicsManager.h"

#include "CollisionManager.h"
#include "Core/GameObject.h"
#include "Systems/Engine/EngineManager.h"
#include "Systems/Engine/LoggingManager.h"
#include "Systems/Physics/Components/Collider2D.h"
#include "Systems/Physics/Components/RigidBody2D.h"
#include "Systems/Physics/Materials/PhysicsMaterial2D.h"

#include <algorithm>

namespace
{
	// Closing speeds below this are treated as resting contact (no bounce) so bouncy materials don't jitter at rest.
	// Unit-dependent: tune to your world-unit scale.
	constexpr float kRestitutionVelocityThreshold = 1.0f;

	/*------------------------------------------------------------------------------------------------------
	| --- ResolveMaterial: Collider's shared material, else the body's, else the global default --- |
	------------------------------------------------------------------------------------------------------*/
	const CE::PhysicsMaterial2D& ResolveMaterial(const CE::Collider2D* pCollider, const CE::RigidBody2D* pBody)
	{
		if (pCollider && pCollider->GetSharedMaterial())
			return *pCollider->GetSharedMaterial();

		if (pBody && pBody->GetSharedMaterial())
			return *pBody->GetSharedMaterial();

		return CE::PhysicsMaterial2D::GetDefault();
	}

	/*-----------------------------------------------------------------------------------------------
	| --- GetContactVelocity: Velocity a body brings to a contact (Static / missing bodies: 0) --- |
	-----------------------------------------------------------------------------------------------*/
	CE::Vector2f GetContactVelocity(const CE::RigidBody2D* pBody)
	{
		if (!pBody || !pBody->IsSimulated() || pBody->GetBodyType() == CE::BodyType2D::Static)
			return CE::Vector2f::Zero();

		return pBody->GetLinearVelocity();
	}

	/*-------------------------------------------------------------------------------------------
	| --- MoveBody: Applies a positional correction, honoring frozen position constraints --- |
	-------------------------------------------------------------------------------------------*/
	void MoveBody(CE::RigidBody2D* pBody, CE::Vector2f delta)
	{
		const CE::BodyConstraints2D constraints = pBody->GetConstraints();

		if (CE::HasFlag(constraints, CE::BodyConstraints2D::FreezePositionX))
			delta.x = 0.0f;

		if (CE::HasFlag(constraints, CE::BodyConstraints2D::FreezePositionY))
			delta.y = 0.0f;

		pBody->SetPosition(pBody->GetPosition() + delta);
		pBody->WakeUp();
	}
}

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-----------------------------------------------------------------------
| --- Constructor: Constructs the PhysicsManager with default values --- |
-----------------------------------------------------------------------*/
CE::PhysicsManager::PhysicsManager()
	: m_pCollisionManager{ nullptr }
	, m_gravity{ 0.0f, 0.0f }
	, m_solverIterations{ kDefaultSolverIterations }
{
}

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
CE::PhysicsManager::~PhysicsManager()
{
	Shutdown();
}

/*------------------------------------------------------------
| --- Initialize: Caches the CollisionManager for stepping --- |
------------------------------------------------------------*/
bool CE::PhysicsManager::Initialize()
{
	m_pCollisionManager = EngineManager::GetInstance().GetCollisionManager();
	if (!m_pCollisionManager)
	{
		CE_LOG("PhysicsManager::Initialize - CollisionManager is unavailable.");
		return false;
	}

	return true;
}

/*-----------------------------------------------------------------------------------------------------
| --- Step: Advances the simulation by one fixed step: integrate, detect, solve, dispatch events --- |
-----------------------------------------------------------------------------------------------------*/
void CE::PhysicsManager::Step(float fixedDeltaTime)
{
	if (!m_pCollisionManager || fixedDeltaTime <= 0.0f)
		return;

	// Stage 1: integrate forces, gravity, damping, and velocity into every body's Transform
	IntegrateBodies(fixedDeltaTime);

	// Stage 2: first detection pass, kept for event dispatch (pre-solve contacts keep resting contacts steady)
	m_eventContacts = m_pCollisionManager->DetectContacts();

	// Stage 3: solve. Pass 1 reuses the first detection; later passes re-detect against the corrected positions.
	const std::vector<Contact2D>* pContacts = &m_eventContacts;
	std::vector<Contact2D> redetected;

	for (int iteration = 0; iteration < m_solverIterations; ++iteration)
	{
		if (!SolveContacts(*pContacts))
			break;

		if (iteration + 1 < m_solverIterations)
		{
			redetected = m_pCollisionManager->DetectContacts();
			pContacts = &redetected;
		}
	}

	// Stage 4: Enter / Update / Exit events from the first (pre-solve) contacts
	m_pCollisionManager->DispatchContactEvents(m_eventContacts);
}

/*-----------------------------------------------------
| --- Shutdown: Drops all registered bodies --- |
-----------------------------------------------------*/
void CE::PhysicsManager::Shutdown()
{
	m_bodies.clear();
	m_bodyByOwner.clear();
	m_eventContacts.clear();
	m_pCollisionManager = nullptr;
}

/*-------------------------------------------------------------------------
| --- AddBody: Registers a body and indexes it by its owning GameObject --- |
-------------------------------------------------------------------------*/
void CE::PhysicsManager::AddBody(RigidBody2D* pBody)
{
	if (!pBody || !pBody->GetOwner())
		return;

	if (std::find(m_bodies.begin(), m_bodies.end(), pBody) != m_bodies.end())
		return;

	const GameObject* pOwner = pBody->GetOwner();
	auto it = m_bodyByOwner.find(pOwner);
	if (it != m_bodyByOwner.end() && it->second != pBody)
	{
		CE_LOG("PhysicsManager::AddBody - '{}' already has a RigidBody2D; ignoring the extra one.", pOwner->GetName());
		return;
	}

	m_bodies.push_back(pBody);
	m_bodyByOwner[pOwner] = pBody;
}

/*------------------------------------------------------------------------------------------------
| --- RemoveBody: Unregisters a body (matches by value: the owner is already null when a --- |
| --- component is removed from its GameObject, so the owner key can't be used here)      --- |
------------------------------------------------------------------------------------------------*/
void CE::PhysicsManager::RemoveBody(RigidBody2D* pBody)
{
	if (!pBody)
		return;

	m_bodies.erase(std::remove(m_bodies.begin(), m_bodies.end(), pBody), m_bodies.end());
	std::erase_if(m_bodyByOwner, [pBody](const auto& entry) { return entry.second == pBody; });
}

/*-----------------------------------------------
| --- GetGravity: Returns the world gravity --- |
-----------------------------------------------*/
const CE::Vector2f& CE::PhysicsManager::GetGravity() const
{
	return m_gravity;
}

/*--------------------------------------------
| --- SetGravity: Sets the world gravity --- |
--------------------------------------------*/
void CE::PhysicsManager::SetGravity(const Vector2f& gravity)
{
	m_gravity = gravity;
}

/*-------------------------------------------------------------------------
| --- GetSolverIterations: Returns the contact-solver passes per step --- |
-------------------------------------------------------------------------*/
int CE::PhysicsManager::GetSolverIterations() const
{
	return m_solverIterations;
}

/*----------------------------------------------------------------------
| --- SetSolverIterations: Sets the contact-solver passes per step --- |
----------------------------------------------------------------------*/
void CE::PhysicsManager::SetSolverIterations(int iterations)
{
	m_solverIterations = std::max(iterations, 1);
}


/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*------------------------------------------------------------------------------------------
| --- FindBody: O(1) lookup of the RigidBody2D on a collider's GameObject, or nullptr --- |
------------------------------------------------------------------------------------------*/
CE::RigidBody2D* CE::PhysicsManager::FindBody(const Collider2D* pCollider) const
{
	if (!pCollider)
		return nullptr;

	auto it = m_bodyByOwner.find(pCollider->GetOwner());
	return (it != m_bodyByOwner.end()) ? it->second : nullptr;
}

/*------------------------------------------------------------------------------------------
| --- GetInverseMass: 1 / mass for simulated Dynamic bodies, 0 for everything else --- |
------------------------------------------------------------------------------------------*/
float CE::PhysicsManager::GetInverseMass(const RigidBody2D* pBody)
{
	if (!pBody || !pBody->IsSimulated() || pBody->GetBodyType() != BodyType2D::Dynamic)
		return 0.0f;

	return 1.0f / pBody->GetMass();		// SetMass clamps to a positive minimum
}

/*---------------------------------------------------------
| --- IntegrateBodies: Steps every registered body --- |
---------------------------------------------------------*/
void CE::PhysicsManager::IntegrateBodies(float fixedDeltaTime)
{
	for (RigidBody2D* pBody : m_bodies)
	{
		pBody->Step(fixedDeltaTime, m_gravity);
	}
}

/*-----------------------------------------------------------------------------------------
| --- SolveContacts: Resolves every non-trigger contact that involves a movable body --- |
-----------------------------------------------------------------------------------------*/
bool CE::PhysicsManager::SolveContacts(const std::vector<Contact2D>& contacts)
{
	bool resolvedAny = false;

	for (const Contact2D& contact : contacts)
	{
		if (contact.isTrigger || contact.depth <= 0.0f)
			continue;

		RigidBody2D* pBodyA = FindBody(contact.pColliderA);
		RigidBody2D* pBodyB = FindBody(contact.pColliderB);

		if (GetInverseMass(pBodyA) + GetInverseMass(pBodyB) <= 0.0f)
			continue;

		ResolvePosition(contact, pBodyA, pBodyB);
		ResolveVelocity(contact, pBodyA, pBodyB);
		resolvedAny = true;
	}

	return resolvedAny;
}

/*----------------------------------------------------------------------------------------------
| --- ResolvePosition: Separates a pair along the MTV, split by inverse mass (static: 0) --- |
----------------------------------------------------------------------------------------------*/
void CE::PhysicsManager::ResolvePosition(const Contact2D& contact, RigidBody2D* pBodyA, RigidBody2D* pBodyB)
{
	const float inverseMassA = GetInverseMass(pBodyA);
	const float inverseMassB = GetInverseMass(pBodyB);
	const float inverseMassSum = inverseMassA + inverseMassB;

	if (inverseMassSum <= 0.0f)
		return;

	const Vector2f mtv = contact.GetMTV();

	if (inverseMassA > 0.0f)
	{
		MoveBody(pBodyA, mtv * (inverseMassA / inverseMassSum));
	}

	if (inverseMassB > 0.0f)
	{
		MoveBody(pBodyB, -mtv * (inverseMassB / inverseMassSum));
	}
}

/*--------------------------------------------------------------------------------------------------------
| --- ResolveVelocity: Removes closing velocity along the normal (with restitution), then applies --- |
| --- Coulomb friction along the tangent, clamped by the normal impulse                            --- |
--------------------------------------------------------------------------------------------------------*/
void CE::PhysicsManager::ResolveVelocity(const Contact2D& contact, RigidBody2D* pBodyA, RigidBody2D* pBodyB)
{
	const float inverseMassA = GetInverseMass(pBodyA);
	const float inverseMassB = GetInverseMass(pBodyB);
	const float inverseMassSum = inverseMassA + inverseMassB;

	if (inverseMassSum <= 0.0f)
		return;

	Vector2f velocityA = GetContactVelocity(pBodyA);
	Vector2f velocityB = GetContactVelocity(pBodyB);
	const Vector2f& normal = contact.normal;						// Points from B to A

	// Positive = separating, negative = closing
	const float closingSpeed = Vector2f::Dot(velocityA - velocityB, normal);
	if (closingSpeed >= 0.0f)
		return;

	const PhysicsMaterial2D& materialA = ResolveMaterial(contact.pColliderA, pBodyA);
	const PhysicsMaterial2D& materialB = ResolveMaterial(contact.pColliderB, pBodyB);

	// Normal impulse (restitution)
	float bounciness = PhysicsMaterial2D::GetCombinedValues(
		materialA.GetBounciness(), materialB.GetBounciness(),
		materialA.GetBounceCombine(), materialB.GetBounceCombine());

	if (-closingSpeed < kRestitutionVelocityThreshold)
		bounciness = 0.0f;

	const float normalImpulse = -(1.0f + bounciness) * closingSpeed / inverseMassSum;

	velocityA += normal * (normalImpulse * inverseMassA);
	velocityB -= normal * (normalImpulse * inverseMassB);

	// Friction impulse along the tangent, measured after the normal impulse
	const Vector2f tangent = normal.Perpendicular();
	const float tangentSpeed = Vector2f::Dot(velocityA - velocityB, tangent);

	const float friction = PhysicsMaterial2D::GetCombinedValues(
		materialA.GetFriction(), materialB.GetFriction(),
		materialA.GetFrictionCombine(), materialB.GetFrictionCombine());

	const float maxFrictionImpulse = friction * normalImpulse;
	const float tangentImpulse = std::clamp(-tangentSpeed / inverseMassSum, -maxFrictionImpulse, maxFrictionImpulse);

	velocityA += tangent * (tangentImpulse * inverseMassA);
	velocityB -= tangent * (tangentImpulse * inverseMassB);

	// Only bodies that can respond receive the new velocity
	if (inverseMassA > 0.0f)
	{
		pBodyA->SetLinearVelocity(velocityA);
	}

	if (inverseMassB > 0.0f)
	{
		pBodyB->SetLinearVelocity(velocityB);
	}
}