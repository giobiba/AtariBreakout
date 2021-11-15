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

void Game::Draw(Renderer& renderer)
{
    for (GameObject& tile : this->Bricks)
        if (!tile.Destroyed)
            tile.Draw(renderer);
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
                        this->Bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture("block"), color));
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

    ResourceManager::LoadTexture("images/image.png", false, "face");
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
}

void Game::Update(float dt) {}

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

}