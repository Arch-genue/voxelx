/**
 * @file physicsengine.h
 * @author Vlad Kartsaev
 * @brief PhysicsEngine class
 * @version 0.1
 * @date 2023-04-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "physics.h"
#include "../gamesystems/gameobject.h"

/**
 * @brief Физический движок
 * ! УСТАРЕЛО !
 */
class PhysicsEngine {
private:
    std::vector<PhysicsObject*> _objects;
    glm::vec3 _gravity;

    // Node* _root;

public:
    PhysicsEngine();
    ~PhysicsEngine();

    glm::vec3 getGravity();

    void addObject(PhysicsObject* object);
    PhysicsObject* getObject(int i);

    void update(float deltaTime);

    bool checkCollision(PhysicsObject* object, glm::vec3& surfacePosition, glm::vec3& surfaceNormal);

    void handleCollision(PhysicsObject* object, glm::vec3 surfacePosition, glm::vec3 surfaceNormal);

    bool raycast(GameObject* gameobject, glm::vec3 pos, glm::vec3 dir, float maxDist, glm::vec3& end, glm::vec3& norm, glm::vec3& iend);
    bool raycast(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, PhysicsObject* physicsobject);

    // Node* rotateLeft(Node* root);
    // Node* rotateRight(Node* root);
    // void flipColors(Node* root);
    // Node* insert(Node* root, BoxCollider* box);
    // Node* findNearestObject(Node* root, BoxCollider* area);

    // Node* getRootNode();
};