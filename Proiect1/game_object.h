#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glad/glad.h>
#include "glm/glm/glm.hpp"

#include "texture.h"
#include "renderer.h"

class GameObject
{
public:
	glm::vec2   Position, Size, Velocity;
	glm::vec3   Color;

	float Rotation;
	bool Destroyed;
	bool IsSolid;

	Texture Sprite;

	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

	void Draw(Renderer& renderer);
};

#endif