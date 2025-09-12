#pragma once

#include <reactphysics3d/reactphysics3d.h>

struct RigidBody {
    rp3d::RigidBody* body = nullptr;

    RigidBody() = default;

    RigidBody(rp3d::PhysicsWorld* world, rp3d::Transform transform) {
        body = world->createRigidBody(transform);
    }
    RigidBody(rp3d::PhysicsWorld* world, glm::vec3 position, rp3d::BodyType bodytype = rp3d::BodyType::STATIC) {
        rp3d::Transform transform;
        transform.setPosition(rp3d::Vector3(position.x, position.y, position.z));
        body = world->createRigidBody(transform);
        body->setType(bodytype);
    }
};