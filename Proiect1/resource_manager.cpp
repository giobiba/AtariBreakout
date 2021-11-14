#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

std::map<std::string, Texture> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;

Shader ResourceManager::LoadShader(const char* vertShaderPath, const char* fragShaderPath, std::string name)
{
	Shaders[name] = loadShaderFromFile(vertShaderPath, fragShaderPath);
	return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

Texture ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	for (auto it : Shaders)
		glDeleteProgram(it.second.ID);

	for (auto it : Textures)
		glDeleteTextures(1, &it.second.id);
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile)
{
	Shader shader = Shader(vShaderFile, fShaderFile);
	return shader;
}

Texture ResourceManager::loadTextureFromFile(const char* file, bool has_alpha)
{
	Texture texture;
	if (has_alpha)
	{
		texture.internal_format = GL_RGBA;
		texture.image_format = GL_RGBA;
	}

	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

	texture.Generate(width, height, data);

	stbi_image_free(data);
	return texture;
}
