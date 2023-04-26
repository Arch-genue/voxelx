#pragma once

#include "gameobject.h"
#include <stdlib.h>
#include "../physics/physicsengine.h"

class GameManager {
    GameObject** gameobjects;
    PhysicsEngine* physicsengine;

    uint16_t size;

    uint16_t _size_incr;
public:
    GameManager(uint16_t size);
    ~GameManager();

    void addGameObject(GameObject* gm);

    void Update();
    void UpdatePhysics(float deltaTime);
};