#pragma once

#include <reactphysics3d/reactphysics3d.h>

struct RigidBody {
    rp3d::RigidBody* body = nullptr;

    RigidBody() = default;

    RigidBody(rp3d::PhysicsWorld* world, rp3d::Transform transform) {
        body = world->createRigidBody(transform);
    }
};