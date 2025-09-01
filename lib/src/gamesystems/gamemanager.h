/**
 * @file gamemanager.h
 * @author Vlad Kartsaev
 * @brief GameManager class implementation
 * @version 0.1
 * @date 2023-04-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <stdlib.h>

#include "physics/physicsengine.h"
#include "gameobject.h"

class GameObject;
class PhysicsEngine;

/**
 * @brief Класс для обработки игровых объектов, сущностей, систем частиц и т.д.
 * 
 */
class GameManager {
private:
    std::vector<GameObject*> _gameobjects;
    uint _gameobject_increment;

    PhysicsEngine* _physicsengine;

    GameManager();
    ~GameManager() = default;
public: 
    // Singleton
    static GameManager& instance() {
        static GameManager inst;
        return inst;
    }

    void addGameObject(GameObject* gameobject);

    PhysicsEngine* getPhysicsEngine();

    void Update(Light & light);
    void UpdatePhysics(float deltaTime);

    std::vector<GameObject*>& getGameObjects() { return _gameobjects; }
    uint32_t getGameObjectsSize() { return _gameobjects.size(); }

    uint getNewID();
    
};