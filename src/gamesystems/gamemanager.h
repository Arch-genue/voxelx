#pragma once

#include "gameobject.h"
#include <stdlib.h>

class GameManager {
    GameObject** gameobjects;
    uint16_t size;

    uint16_t _size_incr;
public:
    GameManager(uint16_t size);
    ~GameManager();

    void addGameObject(GameObject* gm);

    void Update();
    void UpdatePhysics(float deltaTime);
};