#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(glm::vec3 position, float mass, float colliderRadius, float restitution = 0.8f, glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f), bool isStatic = false)
	: m_Position(position), m_Velocity(0.0f), m_Acceleration(0.0f), m_Mass(mass), m_Restitution(restitution), m_Radius(colliderRadius), m_IsStatic(isStatic), m_InvMass(isStatic ? 0.0f : (mass > 0.0f ? 1.0f / mass : 0.0f)), m_Friction(0.0f), m_Normal(normal)
{

}

PhysicsObject::~PhysicsObject()
{

}

void PhysicsObject::update(float deltaTime)
{
	// Upgrade to RK4
	if (this->m_IsStatic)
	{
		return;
	}

	this->m_Velocity += this->m_Acceleration * deltaTime;
	this->m_Position += this->m_Velocity * deltaTime;
}

void PhysicsObject::ApplyForce(const glm::vec3& force)
{
	this->m_Acceleration += force * this->m_InvMass;
}

void PhysicsObject::Reset()
{
	m_Acceleration = glm::vec3(0.0f);
}

void PhysicsObject::setPosition(const glm::vec3& position)
{
	this->m_Position = position;
}

glm::vec3 PhysicsObject::getPosition() const
{
	return this->m_Position;
}