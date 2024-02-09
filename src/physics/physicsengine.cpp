#include "physicsengine.h"

class GameObject;

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
    if (object->getPhysics() == STATIC_PHYSICS or object->getPhysics() == NO_PHYSICS) return false;

    return object->checkGround(surfacePosition, surfaceNormal);
    
    // return false;
}
void PhysicsEngine::handleCollision(PhysicsObject* object, glm::vec3 surfacePosition, glm::vec3 surfaceNormal) {
     // Применяем реакцию на столкновение
    glm::vec3 velocity = object->getVelocity();
    glm::vec3 acceleration = object->getAcceleration();

    // // Изменяем скорость объекта в направлении, перпендикулярном поверхности
    // glm::vec3 normalVelocity = glm::dot(velocity, surfaceNormal) * surfaceNormal;
    // glm::vec3 tangentVelocity = velocity - normalVelocity;
    velocity = glm::vec3(0.0f); //tangentVelocity - 0.8f * normalVelocity;

    // // Изменяем ускорение объекта в направлении, перпендикулярном поверхности
    // glm::vec3 normalAcceleration = glm::dot(acceleration, surfaceNormal) * surfaceNormal;
    // glm::vec3 tangentAcceleration = acceleration - normalAcceleration;
    acceleration = glm::vec3(0.0f);// tangentAcceleration - 0.8f * normalAcceleration;

    // Обновляем скорость и ускорение объекта
    object->setVelocity(velocity);
    object->setAcceleration(acceleration);
    
    GameObject* gmobj = object->getGameObject(); 
    gmobj->setPosition(surfacePosition + surfaceNormal * 0.0f);    
}