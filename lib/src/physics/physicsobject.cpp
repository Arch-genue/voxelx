#include "physicsobject.h"

//! Physics Engine
#include <reactphysics3d/reactphysics3d.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

void PhysicsObject::setPhysicsType(PHYSICS physics) {
    switch (physics) {
        case PHYSICS::KINEMATIC_PHYSICS:
            _rigidbody->setPhysicsType(rp3d::BodyType::KINEMATIC);
            break;
        case PHYSICS::DYNAMIC_PHYSICS:
            _rigidbody->setPhysicsType(rp3d::BodyType::DYNAMIC);
            break;
        default:
            _rigidbody->setPhysicsType(rp3d::BodyType::STATIC);
            break;
    }
}

void PhysicsObject::update(float deltaTime) {
    if (_rigidbody == nullptr) return;

    const rp3d::Vector3& position = _rigidbody->getPosition();
    
    const rp3d::Quaternion& orient = _rigidbody->getOrientation();
    glm::quat glmQuaternion(orient.w, orient.x, orient.y, orient.z);
    
    glm::mat4 rotationMatrix = glm::toMat4(glmQuaternion);

    // getGameObject()->setPosition(glm::vec3(position.x, position.y, position.z));
    // getGameObject()->setRotationMat(rotationMatrix);
}