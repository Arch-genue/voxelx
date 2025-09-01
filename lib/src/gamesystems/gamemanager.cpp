#include "gamemanager.h"

#include <glm/glm.hpp>
#include "../window/input.h"
#include "../loaders/resourcemanager.h"

GameManager::GameManager() {
    _physicsengine = new PhysicsEngine();
    _gameobject_increment = 0;

    Logger::instance().log(LogLevel::INFO, "GameManager", "GameManager initialized");
}

void GameManager::addGameObject(GameObject *gameobject)
{
    _gameobjects.push_back(gameobject);
    _physicsengine->addObject(gameobject->getPhysicsObject());
}

PhysicsEngine* GameManager::getPhysicsEngine() {
    return _physicsengine;
}

void GameManager::Update(Light &light) {
    for(uint16_t i = 0; i < _gameobjects.size(); ++i) {
        Material material;
        material.ambient = glm::vec3(0.1f);
        material.diffuse = glm::vec3(0.8f);
        material.specular = glm::vec3(1.0f);
        material.shininess = 32.0f;
        ResourceManager::getShader("voxel")->uniformLight("light", light);
        ResourceManager::getShader("voxel")->uniformMaterial("material", material);
        
        int16_t remeshed = 0;
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
            Logger::instance().log(LogLevel::VERBOSE, "GameManager", "Remeshed: bryellow<", remeshed ,">");
        }

        _gameobjects[i]->update();
    }
}
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
void GameManager::UpdatePhysics(float deltaTime) {
    static double accumulator = 0.0;
    constexpr double physicsStep = 1.0 / 120.0; // фиксированный шаг = 60 Гц

    accumulator += deltaTime;
    
    while (accumulator >= physicsStep) {
        _physicsengine->update(deltaTime);

        for(uint16_t i = 0; i < _gameobjects.size(); ++i) {
            auto* go = _gameobjects[i];
            if (!go || !go->getPhysicsObject()) continue;
            PhysicsObject* phs = _gameobjects[i]->getPhysicsObject();
    
            _gameobjects[i]->setPosition(phs->getPosition());

            const rp3d::Quaternion& orient = phs->getRotation();
            glm::quat glmQuaternion(orient.w, orient.x, orient.y, orient.z);
            
            glm::mat4 rotationMatrix = glm::toMat4(glmQuaternion);

            // getGameObject()->setPosition(glm::vec3(position.x, position.y, position.z));
            // getGameObject()->setRotationMat(rotationMatrix);
            _gameobjects[i]->setRotationMat(rotationMatrix);

            for (size_t b = 0; b < _gameobjects[i]->get_scripts().size(); b++) {
                ScriptSystem::instance().updateScript(_gameobjects[i]->get_scripts().at(b), _gameobjects[i], deltaTime);
            }
        }
        
        accumulator -= physicsStep;
    }
}

uint GameManager::getNewID() {
    _gameobject_increment = _gameobject_increment + 1;
    return _gameobject_increment;
}
