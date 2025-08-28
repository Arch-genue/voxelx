#include "gamemanager.h"

#include <glm/glm.hpp>
#include "../window/input.h"
#include "../loaders/resourcemanager.h"

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
        Material material;
        material.ambient = glm::vec3(0.1f);
        material.diffuse = glm::vec3(0.8f);
        material.specular = glm::vec3(1.0f);
        material.shininess = 32.0f;
        ResourceManager::getShader("voxel")->uniformLight("light", light);
        ResourceManager::getShader("voxel")->uniformMaterial("material", material);
        
        int remeshed = 0;
        MeshModel* meshmodel = _gameobjects[i]->getMeshModel();
        VoxelModel* voxelmodel = meshmodel->getVoxelModel();
        voxelmodel->forEachChunk([&] (VoxelModel::ChunkType& chunk, const ChunkCoord &chunkPos) {
            if (!chunk._remesh) { return; }
            remeshed++;

            Mesh* newmesh = Renderer::generateMesh(voxelmodel, chunk);
            meshmodel->set(&chunk, newmesh);
            chunk._remesh = false;
        });
        if (remeshed > 0) {
            std::cout << "=Remeshed: " << remeshed << "\n";
        }

        _gameobjects[i]->update();
    }
}

void GameManager::UpdatePhysics(float deltaTime) {
    static double accumulator = 0.0;
    constexpr double physicsStep = 1.0 / 120.0; // фиксированный шаг = 60 Гц

    accumulator += deltaTime;
    
    while (accumulator >= physicsStep) {
        _physicsengine->update(deltaTime);

        for(uint16_t i = 0; i < _gameobjects.size(); i++) {
            if (_gameobjects[i] == nullptr) {
                std::cerr << "Null gameobject in GameManager \n";
                continue;
            }

            if (_gameobjects[i]->getPhysicsObject() == nullptr) continue;

            PhysicsObject* phs = _gameobjects[i]->getPhysicsObject();
            _gameobjects[i]->setPosition(phs->getPosition());
        }
        
        accumulator -= physicsStep;
    }
}

uint GameManager::getNewID() {
    _gameobject_increment = _gameobject_increment + 1;
    return _gameobject_increment;
}
