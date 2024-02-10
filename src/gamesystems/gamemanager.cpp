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

PhysicsEngine* GameManager::getPhysicsEngine() {
    return _physicsengine;
}

void GameManager::Update() {
    for(uint16_t i = 0; i < _gameobjects.size(); i++) {
        _gameobjects[i]->update();
    }
}

void GameManager::UpdatePhysics(float deltaTime) {
    _physicsengine->update(deltaTime);

    for(uint16_t i = 0; i < _gameobjects.size(); i++) {
        if (_gameobjects[i]->getPhysicsObject() == nullptr) continue;

        PhysicsObject* phs = _gameobjects[i]->getPhysicsObject();

        if (_gameobjects[i]->getPhysicsObject()->getPhysics() == DYNAMIC_PHYSICS) { 
            phs->applyForce(_physicsengine->getGravity());
        }

        glm::vec3 surfacePosition, surfaceNormal;
        if (_physicsengine->checkCollision(phs, surfacePosition, surfaceNormal)) {
            _physicsengine->handleCollision(phs, surfacePosition, surfaceNormal);
        }

        glm::vec3 end;
        glm::vec3 norm;
        glm::vec3 iend;

        // if (i + 1 < _gameobjects.size()) {
            // if (_physicsengine->raycast(_gameobjects[i], _gameobjects[i+1]->getPosition(), , 20.0f, end, norm, iend)) {
        // }

        _gameobjects[i]->setPosition(phs->getPosition());
    }
    if (_physicsengine->raycast(_gameobjects[1]->getPosition(), glm::vec3(0.0f, -1.0f, 0.0), _gameobjects[0]->getPhysicsObject())) {
        // std::cout << "hit something" << std::endl;
    } else {
        // std::cout << "didnt hit anything" << std::endl;
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