#pragma once

#include <reactphysics3d/reactphysics3d.h>

class ColliderComponent {
public:
    ColliderComponent(rp3d::CollisionShape* shape) : _shape(shape) {}
    ~ColliderComponent() {
        // delete _collider;
        // delete _shape;
    }
    rp3d::CollisionShape* getShape() const { return _shape; }

    void setCollider(rp3d::Collider* collider) { _collider = collider; }
    rp3d::Collider* getCollider() { return _collider; }
private:
    rp3d::CollisionShape* _shape;
    rp3d::Collider* _collider;
};