#pragma once

#include "physics.h"
#include "../gamesystems/gameobject.h"

class PhysicsEngine {
public:
    std::vector<PhysicsObject*> objects;

    void addObject(PhysicsObject* object);
    PhysicsObject* getObject(int i);

    void update(float deltaTime);

    bool checkCollision(PhysicsObject* object, glm::vec3& surfacePosition, glm::vec3& surfaceNormal);

    void handleCollision(PhysicsObject* object, glm::vec3 surfacePosition, glm::vec3 surfaceNormal);
};