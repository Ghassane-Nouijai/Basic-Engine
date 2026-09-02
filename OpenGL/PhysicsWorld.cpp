#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld() : m_Gravity(0.0f, -9.81f, 0.0f) {}
PhysicsWorld::~PhysicsWorld() {}

void PhysicsWorld::AddPhysicsObject(std::shared_ptr<SimObject> object)
{
	// ✅ Only store the shared_ptr — no raw pointer needed
	m_SimObjects.push_back(object);
}

void PhysicsWorld::Update(float deltaTime)
{
	// 1. Apply gravity
	for (auto& obj : m_SimObjects)
	{
		PhysicsObject& phys = obj->GetPhysicsObject();
		if (!phys.m_IsStatic)
			phys.ApplyForce(m_Gravity * phys.m_Mass);
	}

	// 2. Integrate positions
	for (auto& obj : m_SimObjects)
	{
		PhysicsObject& phys = obj->GetPhysicsObject();
		if (!phys.m_IsStatic)
			phys.update(deltaTime);
	}

	// 3. Resolve collisions
	ResolveCollisions();
	HandleGroundCollisions(0.0f);

	// 4. Reset accelerations
	for (auto& obj : m_SimObjects)
		obj->GetPhysicsObject().Reset();
}

void PhysicsWorld::ResolveCollisions()
{
	for (size_t i = 0; i < m_SimObjects.size(); i++)
		for (size_t j = i + 1; j < m_SimObjects.size(); j++)
		{
			PhysicsObject& a = m_SimObjects[i]->GetPhysicsObject();
			PhysicsObject& b = m_SimObjects[j]->GetPhysicsObject();

			CollisionDetection info = SphereSphereCollision(
				a.getPosition(), a.m_Radius,
				b.getPosition(), b.m_Radius);

			if (info.m_CollisionDetected)
				ResolvePair(a, b, info);
		}
}

void PhysicsWorld::ResolvePair(PhysicsObject& a, PhysicsObject& b, const CollisionDetection& info)
{
	float totalInvMass = a.m_InvMass + b.m_InvMass;
	if (totalInvMass <= 0.0f) return;

	// Positional correction
	glm::vec3 correction = info.m_Normal * (info.m_Depth / totalInvMass);
	if (!a.m_IsStatic) a.setPosition(a.getPosition() - correction * a.m_InvMass);
	if (!b.m_IsStatic) b.setPosition(b.getPosition() + correction * b.m_InvMass);

	// Velocity response
	glm::vec3 relVel = b.m_Velocity - a.m_Velocity;
	float     velAlongNormal = glm::dot(relVel, info.m_Normal);
	if (velAlongNormal > 0.0f) return;

	float     restitution = std::min(a.m_Restitution, b.m_Restitution);
	float     impulseMag = -(1.0f + restitution) * velAlongNormal / totalInvMass;
	glm::vec3 impulse = impulseMag * info.m_Normal;

	if (!a.m_IsStatic) a.m_Velocity -= impulse * a.m_InvMass;
	if (!b.m_IsStatic) b.m_Velocity += impulse * b.m_InvMass;
}

void PhysicsWorld::HandleGroundCollisions(float groundY)
{
	for (auto& obj : m_SimObjects)
	{
		PhysicsObject& phys = obj->GetPhysicsObject();
		if (phys.m_IsStatic) continue;

		CollisionDetection info = SpherePlaneCollision(
			phys.getPosition(), phys.m_Radius,
			glm::vec3(0.0f, 1.0f, 0.0f), groundY);

		if (info.m_CollisionDetected)
		{
			phys.setPosition(phys.getPosition() + info.m_Normal * info.m_Depth);

			float velAlongNormal = glm::dot(phys.m_Velocity, info.m_Normal);
			if (velAlongNormal < 0.0f)
			{
				phys.m_Velocity -= (1.0f + phys.m_Restitution) * velAlongNormal * info.m_Normal;
				phys.m_Velocity.x *= 0.98f;
				phys.m_Velocity.z *= 0.98f;
			}
		}
	}
}