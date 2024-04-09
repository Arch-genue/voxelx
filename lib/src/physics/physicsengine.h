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

//! Physics Engine
#include <reactphysics3d/reactphysics3d.h>

/**
 * @brief Физический движок
 * ! УСТАРЕЛО !
 */
class PhysicsEngine {
private:
    std::vector<PhysicsObject*> _objects;

    reactphysics3d::PhysicsCommon _physicsCommon;
    reactphysics3d::PhysicsWorld* _world;
public:
    PhysicsEngine();
    ~PhysicsEngine();

    reactphysics3d::PhysicsCommon& getPhysicsCommon();
    reactphysics3d::PhysicsWorld* getPhysicsWorld();

    PhysicsObject* createRigidBody(GameObject* gmobj, glm::vec3 size);

    void addObject(PhysicsObject* object);
    PhysicsObject* getObject(int i);

    void update(float deltaTime);
};