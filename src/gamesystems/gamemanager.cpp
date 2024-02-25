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

void GameManager::Update() {
    for(uint16_t i = 0; i < _gameobjects.size(); i++) {
        _gameobjects[i]->update();
    }
}
enum class Color {
    DEFAULT = 0,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    CYAN = 36
};

// Функция для установки цвета текста
void setColor(Color color) {
    std::cout << "\033[" << static_cast<int>(color) << "m";
}

// Функция для сброса цвета текста
void resetColor() {
    std::cout << "\033[0m";
}
bool test(GameObject* obj1, GameObject* obj2) {
    glm::vec3 obj1min = obj1->getPosition();
    glm::vec3 obj2min = obj2->getPosition();

    glm::vec3 obj1max = obj1->getPhysicsObject()->getCollider()->getMax();
    glm::vec3 obj2max = obj2->getPhysicsObject()->getCollider()->getMax();

    // std::cout << "Floor " << obj1min.x             << " " << obj1min.y             << " " << obj1min.z             << "\n";
    // std::cout <<             obj1min.x + obj1max.x << " " << obj1min.y + obj1max.y << " " << obj1min.z + obj1max.z << "\n";
    
    // std::cout << "Apple " << obj2min.x << " " << obj2min.y << " " << obj2min.z << "\n";
    // std::cout <<             obj2min.x + obj2max.x << " " << obj2max.y << " " << obj2min.z + obj2max.z << "\n";

    if (
        obj1min.x <= obj2min.x + obj2max.x && obj1min.x + obj1max.x >= obj2min.x &&
        obj1min.y <= obj2min.y + obj2max.y && obj1min.y + obj1max.y >= obj2min.y &&
        obj1min.z <= obj2min.z + obj2max.z && obj1min.z + obj1max.z >= obj2min.z
    ) {
        return true;
    } else {
        return false;
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

        // // if (i + 1 < _gameobjects.size()) {
        // // if (_physicsengine->raycast(_gameobjects[i], _gameobjects[i+1]->getPosition(), , 20.0f, end, norm, iend)) {
        // // }

        _gameobjects[i]->setPosition(phs->getPosition());
    }
    // if (test(_gameobjects[1], _gameobjects[2])) {
    //     std::cout << "Collision\n";
    // } else {
    //     std::cout << "No collision\n";
    // }
    
    // Node* root = nullptr;
    // for (const auto& box : _gameobjects) {

        
    // }

    // Пример области для поиска ближайшего объекта
    // BoundingBox searchArea = {
    //     1.0f, 2.0f, -1.0f,
    //     2.0f, -1.0f, 2.0f
    // };
    // visualizeTree(_physicsengine->getRootNode());
    
    // std::cout << std::endl;

    // BoxCollider* collider = new BoxCollider(nullptr, glm::vec3(-50.0f, -50.0f, -50.0f), glm::vec3(50.0f, 50.0f, 50.0f));

    // Поиск ближайшего объекта _gameobjects[2]->getPhysicsObject()->getCollider()
    // Node* nearestObject = _physicsengine->findNearestObject(_physicsengine->getRootNode(), collider);

    // if (nearestObject) {
    //     std::cout << "Nearest object: (" << "ID: " << nearestObject->getBoxCollider()->getGameObject()->getID() << " " << nearestObject->getBoxCollider()->getMin().x << ", " << nearestObject->getBoxCollider()->getMin().y << ", " << nearestObject->getBoxCollider()->getMin().z << ") - ";
    //     std::cout <<                       "(" << nearestObject->getBoxCollider()->getMax().x << ", " << nearestObject->getBoxCollider()->getMax().y << ", " << nearestObject->getBoxCollider()->getMax().z << ")" << std::endl;
    //     std::cout << std::endl;
    //     std::cout << "AREA:           (" << "ID: " << _gameobjects[1]->getID() << " " << _gameobjects[1]->getPhysicsObject()->getCollider()->getMin().x << ", " << _gameobjects[1]->getPhysicsObject()->getCollider()->getMin().y << ", " << _gameobjects[1]->getPhysicsObject()->getCollider()->getMin().z << ") - ";
    //     std::cout <<                       "(" << _gameobjects[1]->getPhysicsObject()->getCollider()->getMax().x << ", " << _gameobjects[1]->getPhysicsObject()->getCollider()->getMax().y << ", " << _gameobjects[1]->getPhysicsObject()->getCollider()->getMax().z << ")" << std::endl;
    //     std::cout << std::endl;
    // } else {
    //     std::cout << "No nearest object found" << std::endl;
    // }

    // if (_physicsengine->raycast(_gameobjects[1]->getPosition(), glm::vec3(0.0f, -1.0f, 0.0), _gameobjects[0]->getPhysicsObject())) {
    //     // std::cout << "hit something" << std::endl;
    // } else {
    //     // std::cout << "didnt hit anything" << std::endl;
    // }
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
