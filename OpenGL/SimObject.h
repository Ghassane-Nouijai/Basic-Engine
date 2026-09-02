#pragma once

#include <memory>
#include "IRenderable.h"
#include "PhysicsObject.h"
#include "Shader.h"

class SimObject
{
public:
	SimObject(std::unique_ptr<IRenderable> renderable, PhysicsObject physics)
		: m_Renderable(std::move(renderable)), m_PhysicsObject(std::move(physics))  
	{

	}

	void Draw(Shader& shader)
	{
		m_Renderable->Draw(shader, m_PhysicsObject.getPosition(), m_PhysicsObject.getNormal());
	}

	PhysicsObject& GetPhysicsObject()
	{
		return m_PhysicsObject;
	}

private:
	std::unique_ptr<IRenderable> m_Renderable;
	PhysicsObject                m_PhysicsObject; 
};