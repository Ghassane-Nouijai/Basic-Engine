#pragma once

#include <glm/glm.hpp>

struct CollisionDetection
{
	bool m_CollisionDetected;
	glm::vec3 m_Normal;
	float m_Depth;
};

inline CollisionDetection SphereSphereCollision(const glm::vec3& posA, float radiusA, const glm::vec3& posB, float radiusB)
{
	float distance = glm::length(posA - posB);
	if (distance < (radiusA + radiusB))
	{
		float depth = (radiusA + radiusB) - distance;
		glm::vec3 normal = glm::normalize(posB - posA);
		return { true, normal, depth };
	}
	return { false, glm::vec3(0.0f), 0.0f };
}

inline CollisionDetection SpherePlaneCollision(const glm::vec3& spherePos, float sphereRadius, const glm::vec3& planeNormal, float planeDistance)
{
	float distance = glm::dot(spherePos, planeNormal) - planeDistance;
	if (distance < sphereRadius)
	{
		float depth = sphereRadius - distance;
		glm::vec3 normal = glm::normalize(planeNormal);
		return { true, normal, depth };
	}
	return { false, glm::vec3(0.0f), 0.0f };
}