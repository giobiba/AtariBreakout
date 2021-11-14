#pragma once

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <glad/glad.h>

#include "texture.h"
#include "shader.h"

class ResourceManager
{
public:
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture> Textures;

	static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, std::string name);
	static Shader GetShader(std::string name);

	static Texture LoadTexture(const char* file, bool alpha, std::string name);
	static Texture GetTexture(std::string name);

	static void Clear();

private:
	ResourceManager() { }

	static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile);
	static Texture loadTextureFromFile(const char* file, bool alpha);
};

#endif