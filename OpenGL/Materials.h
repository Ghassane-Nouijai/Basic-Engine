#pragma once
#include <glm/glm.hpp>

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float     shininess;

	static Material Gold() {
		return { {0.24f, 0.19f, 0.07f},
				 {0.75f, 0.60f, 0.22f},
				 {0.62f, 0.55f, 0.36f}, 51.2f };
	}
	static Material Rubber() {
		return { {0.05f, 0.05f, 0.05f},
				 {0.5f,  0.5f,  0.5f },
				 {0.7f,  0.7f,  0.7f }, 10.0f };
	}
};