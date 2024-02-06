#pragma once

#include <stdlib.h>

#include "../physics/physicsengine.h"
#include "../graphics/voxelparticles.h"
#include "gameobject.h"

class PhysicsEngine;
class VoxelParticles;

class GameManager {
    GameObject** gameobjects;
    PhysicsEngine* physicsengine;
    VoxelParticles** voxelparticles;

    uint16_t size;

    //* sys
    uint16_t _gameobjects_size_incr;
    uint16_t _voxparticles_size_incr;
public:
    GameManager(uint16_t size);
    ~GameManager();

    void addGameObject(GameObject* gm);
    void addVoxelParticles(VoxelParticles* voxparticles);

    void Update();
    void UpdatePhysics(float deltaTime);
    void UpdateParticles(float deltaTime);
};