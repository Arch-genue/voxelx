#include "physicsengine.h"

void PhysicsEngine::addObject(PhysicsObject* object) {
    objects.push_back(object);
}
PhysicsObject* PhysicsEngine::getObject(int i) {
    return objects[i];
}

void PhysicsEngine::update(float deltaTime) {
    for (auto& object : objects) {
        object->update(deltaTime);
    }
}

bool PhysicsEngine::checkCollision(PhysicsObject* object, glm::vec3& surfacePosition, glm::vec3& surfaceNormal) {
    // Проверяем столкновение между объектом и поверхностью
    // В этом примере мы считаем, что поверхность находится на высоте 0
    if (object->position.y <= 0) {
        surfacePosition = glm::vec3(object->position.x, 0, object->position.z);
        surfaceNormal = glm::vec3(0, 1, 0);
        object->ground = true;
        return true;
    }
    object->ground = false;
    return false;
}

void PhysicsEngine::handleCollision(PhysicsObject* object, glm::vec3 surfacePosition, glm::vec3 surfaceNormal) {

     // Применяем реакцию на столкновение
    glm::vec3 velocity = object->velocity;
    glm::vec3 acceleration = object->acceleration;

    // // Изменяем скорость объекта в направлении, перпендикулярном поверхности
    // glm::vec3 normalVelocity = glm::dot(velocity, surfaceNormal) * surfaceNormal;
    // glm::vec3 tangentVelocity = velocity - normalVelocity;
    velocity = glm::vec3(0.0f); //tangentVelocity - 0.8f * normalVelocity;

    // // Изменяем ускорение объекта в направлении, перпендикулярном поверхности
    // glm::vec3 normalAcceleration = glm::dot(acceleration, surfaceNormal) * surfaceNormal;
    // glm::vec3 tangentAcceleration = acceleration - normalAcceleration;
    acceleration = glm::vec3(0.0f);// tangentAcceleration - 0.8f * normalAcceleration;

    // Обновляем скорость и ускорение объекта
    object->velocity = velocity;
    object->acceleration = acceleration;
    object->position = (surfacePosition + surfaceNormal * 0.0f);

    // // Применяем реакцию на столкновение
    // glm::vec3 velocity = object->velocity;
    // glm::vec3 acceleration = object->acceleration;

    // // Изменяем скорость объекта в направлении, перпендикулярном поверхности
    // glm::vec3 normalVelocity = glm::dot(velocity, surfaceNormal) * surfaceNormal;
    // glm::vec3 tangentVelocity = velocity - normalVelocity;
    // velocity = tangentVelocity - 0.8f * normalVelocity;

    // // Изменяем ускорение объекта в направлении, перпендикулярном поверхности
    // glm::vec3 normalAcceleration = glm::dot(acceleration, surfaceNormal) * surfaceNormal;
    // glm::vec3 tangentAcceleration = acceleration - normalAcceleration;
    // acceleration = tangentAcceleration - 0.8f * normalAcceleration;

    // // Обновляем скорость и ускорение объекта
    // object->velocity = velocity;
    // object->acceleration = acceleration;
    // object->position = surfacePosition + surfaceNormal * 0.01f;
}