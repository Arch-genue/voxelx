#pragma once

#include "reactphysics3d/reactphysics3d.h"

struct Collider {
    rp3d::Collider* collider;

    Collider() = default;

    Collider(rp3d::RigidBody* body, rp3d::CollisionShape* shape, rp3d::Transform transform_offset) {
        collider = body->addCollider(shape, transform_offset);
    }
};