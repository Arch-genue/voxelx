#include "gamemanager.h"

#include <glm/glm.hpp>
#include "../window/input.h"

#define MOUSE_SPEED 1.0f


GameManager::GameManager() {
    _physicsengine = new PhysicsEngine();
    _gameobject_increment = 0;

    vLogger::eprint("GAMEMGR", "GameManager initialized",  LOGLEVEL::INFO);
}
GameManager::~GameManager() {
    _gameobjects.clear();
    vLogger::eprint("GAMEMGR", "GameObjects deleted",  LOGLEVEL::INFO);
}

void GameManager::addGameObject(GameObject* gameobject) {
    // gameobject->setID(getNewID());
    // gameobject->setGameManager(this);
    
    _gameobjects.push_back(gameobject);
    _physicsengine->addObject(gameobject->getPhysicsObject());
}

PhysicsEngine* GameManager::getPhysicsEngine() {
    return _physicsengine;
}

void GameManager::Update(Light &light) {
    for(uint16_t i = 0; i < _gameobjects.size(); i++) {
        // std::cout << "FSD - " << i << " " << _gameobjects.size() << "\n";
        // std::cout << "GM UPD: " << _gameobjects[i]->getName() << "\n";
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
        // _gameobjects[i]->setPosition(phs->getPosition());
    }
}

uint GameManager::getNewID() {
    _gameobject_increment = _gameobject_increment + 1;
    return _gameobject_increment;
}
