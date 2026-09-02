#pragma once
#include "IRenderable.h"
#include "Shader.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <string>

struct SceneObject
{
	std::unique_ptr<IRenderable> renderable;  
	glm::vec3                    position;
	std::string                  tag;        
};

// Scene.h
#pragma once
#include "SimObject.h"
#include "Shader.h"
#include <vector>
#include <memory>

class Scene
{
private:
	std::vector<std::shared_ptr<SimObject>> m_Objects;
public:
	void Add(std::shared_ptr<SimObject> object)
	{
		m_Objects.push_back(object);
	}

	void Draw(Shader& shader)
	{
		for (auto& obj : m_Objects)
			obj->Draw(shader);
	}
};