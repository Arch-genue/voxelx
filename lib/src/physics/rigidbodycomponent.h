#pragma once

#include <reactphysics3d/reactphysics3d.h>

class RigidBodyComponent {
public:
    RigidBodyComponent(rp3d::RigidBody* body) : body(body) {}

    void setIsDebugEnabled(bool state) { body->setIsDebugEnabled(state); };

    void updateMassProperties() { body->updateMassPropertiesFromColliders(); }

    void setMass(float mass) { body->setMass(rp3d::decimal(mass)); }
    rp3d::decimal getMass() const { return body->getMass(); }

    void setPhysicsType(rp3d::BodyType type) { body->setType(type); };

    void setAngularLockAxisFactor(glm::ivec3 pos) { body->setAngularLockAxisFactor(rp3d::Vector3(pos.x, pos.y, pos.z)); }

    rp3d::Collider* addCollider(rp3d::CollisionShape* shape, const rp3d::Transform& t) { return body->addCollider(shape, t); }
    
    void applyForce(const rp3d::Vector3& force) { body->applyLocalForceAtCenterOfMass(force); }
    void setLinearVelocity(const rp3d::Vector3& velocity) { body->setLinearVelocity(velocity); }
    
    const rp3d::Vector3& getPosition() const { return body->getTransform().getPosition(); }
    const rp3d::Quaternion& getOrientation() const { return body->getTransform().getOrientation(); }
    
    void setTransform(const rp3d::Transform& t) { body->setTransform(t); }
    const rp3d::Transform& getTransform() const { return body->getTransform(); }

private:
    rp3d::RigidBody* body;
};