#pragma once

#ifndef GAME_H
#define GAME_H
#include <vector>
#include <tuple>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm/gtc/matrix_transform.hpp"

#include "game_object.h"

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};
typedef std::tuple<bool, Direction, glm::vec2> Collision;

const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);
const glm::vec2 INIT_BALL_VELOCITY(-200.0f, 200.0f);

const float BALL_RADIUS = 12.5f;

class Game {
public:
    bool Keys[1024];
    bool KeysProcessed[1024];
    unsigned int Width, Height;
    std::vector<GameObject> Bricks;

    bool Finished = false;

    Game(unsigned int width, unsigned int height);
    ~Game();

    void Init();
    void ProcessInput(float dt);
    void Draw(Renderer& renderer);
    void Load(const char* file, unsigned int levelWidth,
        unsigned int levelHeight);
    void Update(float dt);
    void Render();
    void Move(float dt, GameObject* obj);

    void DoCollisions();
};


#endif