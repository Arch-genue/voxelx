#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../gamesystems/gameobject.h"

#include <iostream>
class GameObject;

enum _physicsmodel {
    NO_PHYSICS, 
    STATIC_PHYSICS,
    DYNAMIC_PHYSICS
};

class PhysicsObject {
public:
    _physicsmodel type;

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float mass;

    bool ground;

    PhysicsObject(GameObject* gmobj);
    PhysicsObject(glm::vec3 position = glm::vec3(0), _physicsmodel type = NO_PHYSICS, float mass = 1.0f);
    ~PhysicsObject();

    void setPosition(glm::vec3 position);
    void setType(_physicsmodel type);
    void setMass(float mass);

    void applyForce(glm::vec3 force);
    void explode(uint16_t explodeForce);

    void update(float deltaTime);
private:
    GameObject* pGameObject;
};