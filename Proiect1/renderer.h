#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glad/glad.h>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

#include "Shader.h"
#include "texture.h"

#pragma once
class Renderer
{
public:
	Renderer(Shader& shader);
	~Renderer();

	void DrawSprite(Texture texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color);

	Shader shader;
	unsigned int VAO;
	void initRenderData();
};
#endif