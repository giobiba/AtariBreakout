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

void Game::Init() {}

void Game::Update(float dt) {}

void Game::ProcessInput(float dt) {
    // rotate with a and d
    // go back and front with s and w
}

void Game::Render() {}