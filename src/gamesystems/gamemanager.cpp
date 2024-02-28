#include "gamemanager.h"

#include <iostream>

GameManager::GameManager() {
    _physicsengine = new PhysicsEngine();
    _gameobject_increment = 0;

    errorprint("GAMEMGR", "GameManager initialized",  MSGINFO);
}
GameManager::~GameManager() {
    _gameobjects.clear();
    _voxelparticles.clear();
    errorprint("GAMEMGR", "GameObjects deleted",  MSGINFO);
    errorprint("GAMEMGR", "VoxelParticles deleted",  MSGINFO);
}

void GameManager::addGameObject(GameObject* gameobject) {
    gameobject->setID(getNewID());
    gameobject->setGameManager(this);
    
    _gameobjects.push_back(gameobject);
    _physicsengine->addObject(gameobject->getPhysicsObject());
}
void GameManager::addVoxelParticles(VoxelParticles* voxparticles) {
    _voxelparticles.push_back(voxparticles);
}

PhysicsEngine* GameManager::getPhysicsEngine() {
    return _physicsengine;
}

void GameManager::Update(Light &light) {
    for(uint16_t i = 0; i < _gameobjects.size(); i++) {
        Material material;
        material.ambient = glm::vec3(0.1f);
        material.diffuse = glm::vec3(0.8f);
        material.specular = glm::vec3(1.0f);
        material.shininess = 32.0f;
        ResourceManager::getShader("voxel")->uniformLight("light", light);
        ResourceManager::getShader("voxel")->uniformMaterial("material", material);
        ResourceManager::getShader("voxel")->uniformVec3("aPos", _gameobjects[i]->getPosition());
    
        _gameobjects[i]->update();
    }
}

void GameManager::UpdatePhysics(float deltaTime) {
    _physicsengine->update(deltaTime);

    for(uint16_t i = 0; i < _gameobjects.size(); i++) {
        if (_gameobjects[i] == nullptr) {
            std::cerr << "Null gameobject in GameManager \n";
            continue;
        }

        if (_gameobjects[i]->getPhysicsObject() == nullptr) continue;

        PhysicsObject* phs = _gameobjects[i]->getPhysicsObject();

        if (_gameobjects[i]->getPhysicsObject()->getPhysics() == DYNAMIC_PHYSICS) { 
            phs->applyForce(_physicsengine->getGravity());
        }

        glm::vec3 surfacePosition, surfaceNormal;
        if (_physicsengine->checkCollision(phs, surfacePosition, surfaceNormal)) {
            _physicsengine->handleCollision(phs, surfacePosition, surfaceNormal);
        }

        // glm::vec3 end;
        // glm::vec3 norm;
        // glm::vec3 iend;

        // if (i + 1 < _gameobjects.size()) {
        // if (_physicsengine->raycast(_gameobjects[i], _gameobjects[i+1]->getPosition(), , 20.0f, end, norm, iend)) {
        // }

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

uint GameManager::getNewID() {
    _gameobject_increment = _gameobject_increment + 1;
    return _gameobject_increment;
}
