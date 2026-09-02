#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class PhysicsObject
{
public:
	float m_Mass;
	float m_InvMass;
	float m_Friction;
	float m_Restitution;
	float m_Radius;
	glm::vec3 m_Position;
	glm::vec3 m_Velocity;
	glm::vec3 m_Acceleration;
	glm::vec3 m_AngularVelocity;
	bool m_IsStatic;
	glm::vec3 m_Normal;
public:
	PhysicsObject(glm::vec3 position, float mass, float colliderRadius, float restitution, glm::vec3 normal, bool isStatic);
	~PhysicsObject();
	void update(float deltaTime);
	void ApplyForce(const glm::vec3& force);
	void Reset();
	void setPosition(const glm::vec3& position);
	void setNormal(const glm::vec3& normal);
	glm::vec3 getPosition() const;
	glm::vec3 getNormal() const;
};