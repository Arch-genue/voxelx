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
//! Physics Engine
#include <reactphysics3d/reactphysics3d.h>

#include "physicsobject.h"

class PhysicsObject;
class GameObject;
struct RaycastHit;

/**
 * @brief Физический движок
 */
class PhysicsEngine {
private:
    std::vector<PhysicsObject*> _objects;

    reactphysics3d::PhysicsCommon _physicsCommon;
    reactphysics3d::PhysicsWorld* _world;
public:
    PhysicsEngine();
    ~PhysicsEngine() = default;

    reactphysics3d::PhysicsCommon& getPhysicsCommon();
    reactphysics3d::PhysicsWorld* getPhysicsWorld();

    RaycastHit raycast(const glm::vec3 &origin, const glm::vec3 &dir);

    rp3d::RigidBody *createRigidBody(glm::vec3 position);

    void addObject(PhysicsObject* object) { _objects.push_back(object); }
    PhysicsObject* getObject(int i) { return _objects[i]; }

    void update(float deltaTime);
};