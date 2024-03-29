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

#include "../physics/physicsengine.h"
#include "../graphics/voxelparticles.h"
#include "gameobject.h"
#include "linesobject.h"

class PhysicsEngine;
class VoxelParticles;

/**
 * @brief Класс для обработки игровых объектов, сущностей, систем частиц и т.д.
 * 
 */
class GameManager {
    public:
        GameManager();
        ~GameManager();

        void addGameObject(GameObject* gameobject);
        void addVoxelParticles(VoxelParticles* voxelparticles);

        PhysicsEngine* getPhysicsEngine();

        void Update(Light & light);
        void UpdatePhysics(float deltaTime);
        void UpdateParticles(float deltaTime);

        uint32_t getGameObjectsSize() {
            return _gameobjects.size();
        }

        uint32_t getVoxelParticlesSize() {
            return _voxelparticles.size();
        }

        void clearParticles();

        uint getNewID();
    private:
        std::vector<GameObject*> _gameobjects;
        std::vector<VoxelParticles*> _voxelparticles;
        uint _gameobject_increment;

        PhysicsEngine* _physicsengine;
};