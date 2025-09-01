#include "physicsobject.h"

//! Physics Engine
#include <reactphysics3d/reactphysics3d.h>
// #include <glm/gtc/quaternion.hpp>
// #include <glm/gtx/quaternion.hpp>

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

RaycastHit PhysicsObject::raycast(const glm::vec3& origin, const glm::vec3& dir) {
    rp3d::Ray ray(
        rp3d::Vector3(origin.x, origin.y, origin.z),
        rp3d::Vector3(dir.x, dir.y, dir.z) // нормализуем
    );

    // std::cout << rp3d::Vector3(dir.x, dir.y, dir.z).y  << "\n";

    // std::cout << "Raycasted: " << origin.y << " " << dir.y << "\n";

    // PhysicsRaycastCallback callback;
    rp3d::RaycastInfo rayinfo;

    // this->->->raycast(ray, &callback);
    bool raycast = this->getCollider()->getCollider()->raycast(ray, rayinfo);

    RaycastHit result{};
    if (raycast) {
        result.hit = true;
        result.hitObject = static_cast<GameObject*>(rayinfo.collider->getUserData());
        result.point = glm::vec3(rayinfo.worldPoint.x, rayinfo.worldPoint.y, rayinfo.worldPoint.z);
        result.normal = glm::vec3(rayinfo.worldNormal.x, rayinfo.worldNormal.y, rayinfo.worldNormal.z);
    } else {
        result.hit = false;
        result.hitObject = nullptr;
    }
    return result;
}

void PhysicsObject::update(float deltaTime) {
    // if (_rigidbody == nullptr) return;

    // const rp3d::Vector3& position = _rigidbody->getPosition();
    
    // const rp3d::Quaternion& orient = _rigidbody->getOrientation();
    // glm::quat glmQuaternion(orient.w, orient.x, orient.y, orient.z);
    
    // glm::mat4 rotationMatrix = glm::toMat4(glmQuaternion);

    // // getGameObject()->setPosition(glm::vec3(position.x, position.y, position.z));
    // // getGameObject()->setRotationMat(rotationMatrix);
}