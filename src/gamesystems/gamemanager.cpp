#include "gamemanager.h"

#include <iostream>


GameManager::GameManager(){
    _physicsengine = new PhysicsEngine();
}
GameManager::~GameManager() {
    _gameobjects.clear();
    _voxelparticles.clear();
}

void GameManager::addGameObject(GameObject* gameobject) {
    gameobject->setGameManager(this);
    
    _gameobjects.push_back(gameobject);
    if (gameobject->getPhysicsObject() != nullptr) { 
        _physicsengine->addObject(gameobject->getPhysicsObject());
    }
}
void GameManager::addVoxelParticles(VoxelParticles* voxparticles) {
    _voxelparticles.push_back(voxparticles);
}

void GameManager::Update() {
    for(uint16_t i = 0; i < _gameobjects.size(); i++) {
        _gameobjects[i]->update();
    }
}

void GameManager::UpdatePhysics(float deltaTime) {
    glm::vec3 gravity(0, -9.81f, 0);
    _physicsengine->update(deltaTime);

    for(uint16_t i = 0; i < _gameobjects.size(); i++) {
        if (_gameobjects[i]->getPhysicsObject() == nullptr) continue;

        PhysicsObject* phs = _gameobjects[i]->getPhysicsObject();

        if (_gameobjects[i]->getPhysicsObject()->getPhysics() == DYNAMIC_PHYSICS) { 
            phs->applyForce(gravity);
        }

        glm::vec3 surfacePosition, surfaceNormal;
        if (_physicsengine->checkCollision(phs, surfacePosition, surfaceNormal)) {
            _physicsengine->handleCollision(phs, surfacePosition, surfaceNormal);
        }
        _gameobjects[i]->setPosition(phs->getPosition());
    }
}

void GameManager::UpdateParticles(float deltaTime) {
    for (uint16_t i = 0; i < _voxelparticles.size(); i++) {
        _voxelparticles[i]->update(deltaTime);
    }
}

void GameManager::clearParticles() {
    for (uint64_t i = 0; i < _voxelparticles.size(); i++) {
        delete &_voxelparticles[i];
    }
    _voxelparticles.clear();
}