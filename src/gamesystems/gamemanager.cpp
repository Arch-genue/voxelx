#include "gamemanager.h"

#include <iostream>


GameManager::GameManager(uint16_t size) : size(size) {
    _gameobjects_size_incr = 0;
    _voxparticles_size_incr = 0;

    gameobjects = new GameObject*[size];
    physicsengine = new PhysicsEngine();
    voxelparticles = new VoxelParticles*[50];
}
GameManager::~GameManager() {
    for(uint16_t i = 0; i < size; i++) delete gameobjects[i];
    for(uint16_t i = 0; i < 50; i++) delete voxelparticles[i];
}

void GameManager::addGameObject(GameObject* gameobj) {
    gameobjects[_gameobjects_size_incr] = gameobj;
    gameobjects[_gameobjects_size_incr]->setGameManager(this);
    if (gameobj->getPhysics() != nullptr) { 
        physicsengine->addObject(gameobj->getPhysics());
    }
    _gameobjects_size_incr++;
}
void GameManager::addVoxelParticles(VoxelParticles* voxparticles) {
    voxelparticles[_voxparticles_size_incr] = voxparticles;
    _voxparticles_size_incr++;
}

void GameManager::Update() {
    for(uint16_t i = 0; i < _gameobjects_size_incr; i++) {
        gameobjects[i]->draw();
    }
}

void GameManager::UpdatePhysics(float deltaTime) {
    glm::vec3 gravity(0, -9.81f, 0);
    physicsengine->update(deltaTime);

    for(uint16_t i = 0; i < _gameobjects_size_incr; i++) {
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

void GameManager::UpdateParticles(float deltaTime) {
    for (uint16_t i = 0; i < _voxparticles_size_incr; i++) {
        voxelparticles[i]->draw(deltaTime);
    } 
}