#pragma once

#include <iostream>
#include "game.h"
#include "resource_manager.h"
#include "renderer.h"
#include "game_object.h"

GameObject* player;
Renderer* renderer;

Game::Game(unsigned int width, unsigned int height)
    : Keys(), KeysProcessed(), Width(width), Height(height) { }

Game::~Game() {
    delete renderer;
    delete player;
}

void Game::Init() 
{
    // loadam shaders
    ResourceManager::LoadShader("sprite.vert", "sprite.frag", "sprite");

    // configure shaders
    glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(Width), static_cast<float>(Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").use().setInt("sprite", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", proj);

    renderer = new Renderer(ResourceManager::GetShader("sprite").use());

    ResourceManager::LoadTexture("images/image.png", false, "face");
}

void Game::Update(float dt) {}

void Game::ProcessInput(float dt) {
    // rotate with a and d
    // go back and front with s and w
}

void Game::Render() 
{
    renderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(-0.5f, -0.5f), glm::vec2(1.0f, 1.0f));
}