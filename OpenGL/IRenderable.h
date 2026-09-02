#pragma once
#include <glm/glm.hpp>
#include "Shader.h"

class IRenderable
{
public:
	virtual void Draw(Shader& shader, const glm::vec3& position, glm::vec3 normal) = 0;
	virtual ~IRenderable() = default;
};