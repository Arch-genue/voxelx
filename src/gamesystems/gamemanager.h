#pragma once

#include <stdlib.h>

#include "../physics/physicsengine.h"
#include "../graphics/voxelparticles.h"
#include "gameobject.h"

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

        void Update();
        void UpdatePhysics(float deltaTime);
        void UpdateParticles(float deltaTime);

        void clearParticles();
    
    private:
        std::vector<GameObject*> _gameobjects;
        std::vector<VoxelParticles*> _voxelparticles;

        PhysicsEngine* _physicsengine;
};