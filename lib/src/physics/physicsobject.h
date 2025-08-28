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

#include <glm/glm.hpp>
#include "../utilities/utils.h"
#include "../utilities/logger.h"

//! Physics Engine
#include <reactphysics3d/reactphysics3d.h>
#include "rigidbodycomponent.h"
#include "collidercomponent.h"
#include "gamesystems/gameobject.h"

class GameObject;

enum PHYSICS {
    STATIC_PHYSICS,
    KINEMATIC_PHYSICS, 
    DYNAMIC_PHYSICS
};

class PhysicsObject {
public:
    PhysicsObject(GameObject* gameobject): _gameobject(gameobject) {}
    ~PhysicsObject() = default;

    void setRigidBody(std::unique_ptr<RigidBodyComponent> rb) { _rigidbody = std::move(rb); _rigidbody->setIsDebugEnabled(true); }
    void setCollider(std::unique_ptr<ColliderComponent> col) { _collider = std::move(col); this->attachCollider(); }

    void attachCollider() {
        rp3d::Collider* collider = _rigidbody->addCollider(_collider->getShape(), rp3d::Transform::identity());
        rp3d::Material& mat = collider->getMaterial();
        mat.setBounciness (0); 
        mat.setFrictionCoefficient (1.0);
        mat.setMassDensity(10);
        // _rigidbody->updateMassProperties(); // обновляем массу
        std::cout << "MGNG: " << collider << " " << this->_gameobject << "\n";
        collider->setUserData(this->_gameobject);
        _collider->setCollider(collider);
    }

    RigidBodyComponent* getRigidBody() const { return _rigidbody.get(); }
    ColliderComponent* getCollider() const { return _collider.get(); }

    void setPhysicsType(PHYSICS physics);

    void setPosition(glm::vec3 position) {
        if (_rigidbody) {
            rp3d::Transform t = _rigidbody->getTransform();
            t.setPosition(rp3d::Vector3(position.x, position.y, position.z));
            _rigidbody->setTransform(t);
        }
    }
    glm::vec3 getPosition() const { 
        if (_rigidbody) {
            rp3d::Vector3 pos = _rigidbody->getPosition();
            return glm::vec3(pos.x, pos.y, pos.z);
        }
        return glm::vec3(0);
    }

    void setRotation(const rp3d::Quaternion& rot) {
        if (_rigidbody) {
            rp3d::Transform t = _rigidbody->getTransform();
            t.setOrientation(rot);
            _rigidbody->setTransform(t);
        }
    }
    rp3d::Quaternion getRotation() const { return _rigidbody ? _rigidbody->getTransform().getOrientation() : rp3d::Quaternion::identity(); }

    void setLinearVelocity(glm::vec3 velocity) {
        if (_rigidbody) {
            _rigidbody->setLinearVelocity(rp3d::Vector3(velocity.x, velocity.y, velocity.z));
        }
    }
    void applyForce(glm::vec3 force) {
        if (_rigidbody) {
            _rigidbody->applyForce(rp3d::Vector3(force.x, force.y, force.z));
        }
    }

    void update(float deltaTime);

    void setIsGround(bool ground) { _ground = ground; }
    bool isGrounded() const { return _ground; }

private:
    GameObject* _gameobject;

    std::unique_ptr<RigidBodyComponent> _rigidbody;
    std::unique_ptr<ColliderComponent> _collider;
    bool _ground;
};