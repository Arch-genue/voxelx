#pragma once

#include <vector>
#include <glm/glm.hpp>
// #include "../gamesystems/gameobject.h"
#include "boxcollider.h"

#include <iostream>

class GameObject;

enum PHYSICS {
    NO_PHYSICS, 
    STATIC_PHYSICS,
    DYNAMIC_PHYSICS
};

class PhysicsObject {
public:
    PhysicsObject(GameObject* gmobj, BoxCollider* collider, float mass = 1.0f);
    PhysicsObject(glm::vec3 position = glm::vec3(0), PHYSICS physics = NO_PHYSICS, float mass = 1.0f);
    ~PhysicsObject();

    void setCollider(BoxCollider* collider);
    BoxCollider* getCollider();

    GameObject* getGameObject();

    void setPosition(glm::vec3 position);
    glm::vec3 getPosition();

    void setVelocity(glm::vec3 velocity);
    glm::vec3 getVelocity();

    void setAcceleration(glm::vec3 acceleration);
    glm::vec3 getAcceleration();

    void setPhysics(PHYSICS type);
    PHYSICS getPhysics();

    void setMass(float mass);
    float getMass();

    void applyForce(glm::vec3 force);
    void stopForce();

    void explode(uint16_t explodeForce);

    bool isGrounded();

    bool checkGround(glm::vec3& position, glm::vec3& normal);
    
    void update(float deltaTime);

    float* getVertices();
private:
    GameObject* _gameobject;
    PHYSICS _physics;
    BoxCollider* _collider;

    glm::vec3 _position;
    glm::vec3 _velocity;
    glm::vec3 _acceleration;

    float _mass;
    bool _ground;
};