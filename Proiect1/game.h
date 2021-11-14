#pragma once

#ifndef GAME_H
#define GAME_H
#include <vector>
#include <tuple>

#include <vector>
#include <tuple>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm/gtc/matrix_transform.hpp"

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);

class Game {
public:
    bool Keys[1024];
    bool KeysProcessed[1024];
    unsigned int Width, Height;

    Game(unsigned int width, unsigned int height);
    ~Game();

    void Init();
    void ProcessInput(float dt);

    void Update(float dt);
    void Render();
};


#endif