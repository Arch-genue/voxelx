#include "gamemanager.h"

#include <iostream>

GameManager::GameManager(uint16_t size) {
    this->size = size;
    _size_incr = 0;
    gameobjects = new GameObject*[size];

    physicsengine = new PhysicsEngine();
}
GameManager::~GameManager() {
    for(uint16_t i = 0; i < _size_incr; i++) delete gameobjects[i];
}

void GameManager::addGameObject(GameObject* gameobj) {
    gameobjects[_size_incr] = gameobj;
    if (gameobj->getPhysics() != nullptr) { 
        physicsengine->addObject(gameobj->getPhysics());
    }
    _size_incr++;
}

void GameManager::UpdatePhysics(float deltaTime) {
    glm::vec3 gravity(0, -9.81f, 0);
    physicsengine->update(deltaTime);

    for(uint16_t i = 0; i < _size_incr; i++) {
        if (gameobjects[i]->getPhysics() == nullptr) continue;

        PhysicsObject* phs = gameobjects[i]->getPhysics();

        if (gameobjects[i]->getPhysics()->type == DYNAMIC_PHYSICS) { 
            phs->applyForce(gravity);
        }

        glm::vec3 surfacePosition, surfaceNormal;
        if (physicsengine->checkCollision(phs, surfacePosition, surfaceNormal)) {
            physicsengine->handleCollision(phs, surfacePosition, surfaceNormal);
        }
        gameobjects[i]->setPosition(phs->position);
    }
}
void GameManager::Update() {
    for(uint16_t i = 0; i < _size_incr; i++) {
        gameobjects[i]->draw();
    }
}