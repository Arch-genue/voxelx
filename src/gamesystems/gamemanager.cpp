#include "gamemanager.h"

#include <iostream>

GameManager::GameManager(uint16_t size) {
    this->size = size;
    _size_incr = 0;
    gameobjects = new GameObject*[size];
}
GameManager::~GameManager() {
    for(uint16_t i = 0; i < _size_incr; i++) delete gameobjects[i];
}

void GameManager::addGameObject(GameObject* gm) {
    gameobjects[_size_incr] = gm;
    _size_incr++;
}

void GameManager::UpdatePhysics(float deltaTime) {
    for(uint16_t i = 0; i < _size_incr; i++) {
        gameobjects[i]->updatePhysics(deltaTime);

        bool physobj =  (gameobjects[i]->getRigidBody() && gameobjects[i]->getCollision());
        for(uint16_t i2 = 0; i2 < _size_incr; i2++) {
            if (i == i2) continue;
            if (physobj && gameobjects[i2]->getCollision()) {
                gameobjects[i]->checkCollision(gameobjects[i2]->getBBOX()).y;
                if (!gameobjects[i]->checkGround()) {
                    gameobjects[i]->applyForce(vec3(0, -9.8, 0));
                } else {
                    gameobjects[i]->setVelocity(glm::vec3(0, 0, 0));
                    gameobjects[i]->setAcceleration(glm::vec3(0, 0, 0));
                }
            }
        }
    }
}
void GameManager::Update() {
    for(uint16_t i = 0; i < _size_incr; i++) {
        gameobjects[i]->draw();
    }
}