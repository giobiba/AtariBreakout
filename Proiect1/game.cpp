#pragma once

#include <iostream>
#include "game.h"
#include "resource_manager.h"
#include "renderer.h"
#include "game_object.h"

GameObject* player, *ball;
Renderer* renderer;

Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
    glm::vec2(0.0f, 1.0f), // sus
    glm::vec2(1.0f, 0.0f), // dreapta
    glm::vec2(0.0f, -1.0f), // jos
    glm::vec2(-1.0f, 0.0f) // stanga
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}

Game::Game(unsigned int width, unsigned int height)
    : Keys(), KeysProcessed(), Width(width), Height(height) { }

Game::~Game() {
    delete renderer;
    delete player, ball;
}

Collision CheckCollision(GameObject& ball, GameObject& two)
{
    glm::vec2 ball_center(ball.Position + ball.Size / 2.0f);
    glm::vec2 half_box(two.Size.x / 2.0f, two.Size.y / 2.0f);
    glm::vec2 box_center(two.Position.x + half_box.x, two.Position.y + half_box.y);
    
    glm::vec2 difference = ball_center - box_center;
    glm::vec2 clamped = glm::clamp(difference, -half_box, half_box);

    glm::vec2 closest = box_center + clamped;


    glm::vec2 diff = closest - ball_center;
    if (glm::length(diff) < BALL_RADIUS)
        return std::make_tuple(true, VectorDirection(diff), diff);
    else 
        return std::make_tuple(false, UP, glm::vec2(0.0f));

}

void Game::Draw(Renderer& renderer)
{
    for (GameObject& brick : this->Bricks)
        if (!brick.Destroyed) 
            brick.Draw(renderer);            
}

void Game::Load(const char* file, unsigned int levelWidth,
    unsigned int levelHeight)
{
    this->Bricks.clear();
    // load from file
    unsigned int tileCode;

    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if (fstream)
    {
        while (std::getline(fstream, line)) //citim fiecare linie
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode) // citim fiecare valoare din fisierul.lvl
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
        {
            unsigned int height = tileData.size();
            unsigned int width = tileData[0].size(); 
            float unit_width = levelWidth / static_cast<float>(width), unit_height = levelHeight / height;
            // initializam tile-urile	
            for (unsigned int y = 0; y < height; ++y)
            {
                for (unsigned int x = 0; x < width; ++x)
                {
                    // verificam tipul de tile
                    if (tileData[y][x] == 1) // solid
                    {
                        glm::vec2 pos(unit_width * x, unit_height * y);
                        glm::vec2 size(unit_width, unit_height);
                        GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
                        obj.IsSolid = true;
                        this->Bricks.push_back(obj);
                    }
                    else if (tileData[y][x] > 1)	// non-solid; punem culoare in functie de nivelul la care se afla 
                    {
                        glm::vec3 color = glm::vec3(1.0f); // original: white
                        if (tileData[y][x] == 2)
                            color = glm::vec3(0.2f, 0.6f, 1.0f);
                        else if (tileData[y][x] == 3)
                            color = glm::vec3(0.0f, 0.7f, 0.0f);
                        else if (tileData[y][x] == 4)
                            color = glm::vec3(0.8f, 0.8f, 0.4f);
                        else if (tileData[y][x] == 5)
                            color = glm::vec3(1.0f, 0.5f, 0.0f);

                        glm::vec2 pos(unit_width * x, unit_height * y);
                        glm::vec2 size(unit_width, unit_height);
                       
                        this->Bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture("block"), color, glm::vec2(-35.0f, 35.0f)));
                    }
                }
            }
        }

    }
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

    ResourceManager::LoadTexture("images/ball.png", true, "ball");
    ResourceManager::LoadTexture("images/background.jpg", false, "background");
    ResourceManager::LoadTexture("images/paddle.png", true, "paddle");
    ResourceManager::LoadTexture("images/block.png",
        false, "block");
    ResourceManager::LoadTexture("images/block_solid.png",false,"block_solid");

    this->Load("levels/one.lvl", Width, Height / 2);

    glm::vec2 playerPos = glm::vec2(Width / 2.0f - PLAYER_SIZE.x / 2.0f,
        Height - PLAYER_SIZE.y);
    player = new GameObject(playerPos, PLAYER_SIZE,
        ResourceManager::GetTexture("paddle"));

    glm::vec2 ballPos = glm::vec2(Width / 2.0f ,
        Height / 2.0f);
    ball = new GameObject(ballPos, glm::vec2(2.0f * BALL_RADIUS, 2.0f * BALL_RADIUS), ResourceManager::GetTexture("ball"), glm::vec3(1.0f), INIT_BALL_VELOCITY);
}

void Game::Update(float dt) {
    
    this->Move(dt, ball);
    this->DoCollisions();
}

void Game::Move(float dt, GameObject* obj)
{
    obj->Position += obj->Velocity * dt;

    if (obj->Position.x <= 0.0f)
    {
        obj->Velocity.x = -ball->Velocity.x;
        obj->Position.x = 0.0f;
    }
    else if (obj->Position.x + obj->Size.x >= Width)
    {
        obj->Velocity.x = -obj->Velocity.x;
        obj->Position.x = Width - obj->Size.x;
    }
    if (obj->Position.y <= 0.0f)
    {
        obj->Velocity.y = -ball->Velocity.y;
        obj->Position.y = 0.0f;
    }
    else if (obj->Position.y + obj->Size.y >= Height)
    {
        this->Finished = true;
    }
}

void Game::ProcessInput(float dt) {

    float velocity = PLAYER_VELOCITY * dt;
    // mutam player-ul cu a sau d
    if (Keys[GLFW_KEY_A])
    {
        if (player->Position.x >= 0.0f)
            player->Position.x -= velocity;
    }
    if (Keys[GLFW_KEY_D])
    {
        if (player->Position.x <= Width - player->Size.x)
            player->Position.x += velocity;
    }
}

void Game::Render() 
{
    renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(Width, Height),0.0f);

    this->Draw(*renderer);
    player->Draw(*renderer);
    ball->Draw(*renderer);

}

void Game::DoCollisions()
{
    for (GameObject& brick : Bricks)
    {
        if (!brick.Destroyed)
        {
            Collision collision = CheckCollision(*ball, brick);
            if (std::get<0>(collision))
            {

                Direction dir = std::get<1>(collision);
                glm::vec2 diff = std::get<2>(collision);

                if (!brick.IsSolid) 
                    brick.Destroyed = true;

                if (dir == LEFT || dir == RIGHT) {
                    ball->Velocity.x = -ball->Velocity.x;

                    float penetration = BALL_RADIUS -
                        std::abs(diff.x);

                    if (dir == LEFT)
                        ball->Position.x += penetration;
                    else
                        ball->Position.x -= penetration;
                }
                else {
                    ball->Velocity.y = -ball->Velocity.y;

                    float penetration = BALL_RADIUS -
                        std::abs(diff.y);

                    if (dir == UP)
                        ball->Position.y -= penetration;
                    else
                        ball->Position.y += penetration; 
                }
                    
            }
        }
    }
    Collision player_collision = CheckCollision(*ball, *player);

    if (std::get<0>(player_collision))
    {
        float center_board = player->Position.x + player->Size.x / 2.0f;
        float dist = (ball->Position.x + BALL_RADIUS) - center_board;
        float perc = dist / (player->Size.x / 2.0f);

        glm::vec2 oldVelocity = ball->Velocity;
        ball->Velocity.x = INIT_BALL_VELOCITY.x * perc * 2.0f;

        ball->Velocity = glm::normalize(ball->Velocity) * glm::length(oldVelocity);
        
        ball->Velocity.y = -1.0f * abs(ball->Velocity.y);
    }
}

