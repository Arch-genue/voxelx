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
public:
    std::vector<PhysicsObject*> objects;

    void addObject(PhysicsObject* object);
    PhysicsObject* getObject(int i);

    void update(float deltaTime);

    bool checkCollision(PhysicsObject* object, glm::vec3& surfacePosition, glm::vec3& surfaceNormal);

    void handleCollision(PhysicsObject* object, glm::vec3 surfacePosition, glm::vec3 surfaceNormal);
};