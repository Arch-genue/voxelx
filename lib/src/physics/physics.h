/**
 * @file physics.h
 * @author Vlad Kartsaev
 * @brief 
 * @version 0.8
 * @date 2023-04-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../utilities/utils.h"
#include "../utilities/logger.h"

//! Physics Engine
#include <reactphysics3d/reactphysics3d.h>

class GameObject;

enum PHYSICS {
    STATIC_PHYSICS,
    KINEMATIC_PHYSICS, 
    DYNAMIC_PHYSICS
};

class PhysicsObject {
private:
    GameObject* _gameobject;

    bool _ground;

    rp3d::RigidBody* _rigidbody;
public:
    PhysicsObject(GameObject* gmobj, rp3d::RigidBody* rigidbody);
    ~PhysicsObject();

    GameObject* getGameObject();
    rp3d::RigidBody* getRigidBody();

    void setType(PHYSICS physics);

    void setPosition(glm::vec3 position);
    void setVelocity(glm::vec3 velocity);
    void applyForce(glm::vec3 force);

    void setMass(float mass);
    float getMass();

    void setIsGround(bool ground);
    bool isGrounded();
    
    void update(float deltaTime);
};