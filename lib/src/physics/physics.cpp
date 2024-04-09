#include "physics.h"
#include "../gamesystems/gameobject.h"

//! Physics Engine
#include <reactphysics3d/reactphysics3d.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class GameObject;


PhysicsObject::PhysicsObject(GameObject *gmobj, rp3d::RigidBody* rigidbody)
    : _gameobject(gmobj), _rigidbody(rigidbody) {}
PhysicsObject::~PhysicsObject() {}

GameObject *PhysicsObject::getGameObject() {
    return _gameobject;
}

rp3d::RigidBody* PhysicsObject::getRigidBody() {
    return _rigidbody;
}

void PhysicsObject::setType(PHYSICS physics) {
    switch (physics) {
        case PHYSICS::STATIC_PHYSICS:
            _rigidbody->setType(rp3d::BodyType::STATIC);
            break;
        case PHYSICS::KINEMATIC_PHYSICS:
            _rigidbody->setType(rp3d::BodyType::KINEMATIC);
            break;
        case PHYSICS::DYNAMIC_PHYSICS:
            _rigidbody->setType(rp3d::BodyType::DYNAMIC);
            break;
        default:
            _rigidbody->setType(rp3d::BodyType::STATIC);
            break;
    }
}

void PhysicsObject::setPosition(glm::vec3 position) {
    const rp3d::Quaternion quat = _rigidbody->getTransform().getOrientation();
    const rp3d::Vector3 vect(position.x, position.y, position.z);
    rp3d::Transform transform(vect, quat);
    _rigidbody->setTransform(transform);
}

void PhysicsObject::setVelocity(glm::vec3 velocity) {
    _rigidbody->setLinearVelocity(rp3d::Vector3(velocity.x, velocity.y, velocity.z));
}
void PhysicsObject::applyForce(glm::vec3 force) {
    _rigidbody->applyLocalForceAtCenterOfMass(rp3d::Vector3(force.x, force.y, force.z));
}

void PhysicsObject::setMass(float mass) {
    _rigidbody->setMass(mass);
}
float PhysicsObject::getMass() {
    return _rigidbody->getMass();
}


void PhysicsObject::setIsGround(bool ground) {
    _ground = ground;
}

bool PhysicsObject::isGrounded() {
    return _ground;
}

void PhysicsObject::update(float deltaTime) {
    const rp3d::Vector3& position = _rigidbody->getTransform().getPosition();
    
    const rp3d::Quaternion& orient = _rigidbody->getTransform().getOrientation();
    glm::quat glmQuaternion(orient.w, orient.x, orient.y, orient.z);
    
    glm::mat4 rotationMatrix = glm::toMat4(glmQuaternion);

    getGameObject()->setPosition(glm::vec3(position.x, position.y, position.z));
    getGameObject()->setRotationMat(rotationMatrix);
}