#include "physics/physicsengine.h"

class PhysicsApi {
public:
    static void reg(sol::state& lua) {
        lua.new_usertype<PhysicsObject>("PhysicsObject",
            "setPosition", &PhysicsObject::setPosition,
            "getPosition", &PhysicsObject::getPosition,
            "setLinearVelocity", &PhysicsObject::setLinearVelocity,
            "setPhysicsType", &PhysicsObject::setPhysicsType,
            "applyForce", &PhysicsObject::applyForce,
            "getRigidBody", &PhysicsObject::getRigidBody,
            "getCollider", &PhysicsObject::getCollider,
            "setIsGround", &PhysicsObject::setIsGround,
            "isGrounded", &PhysicsObject::isGrounded,
            "raycast", &PhysicsObject::raycast
        );
        lua.new_usertype<rp3d::Material>("Material",
            "setBounciness", &rp3d::Material::setBounciness,
            "setMassDensity", &rp3d::Material::setMassDensity,
            "setFrictionCoefficient", &rp3d::Material::setFrictionCoefficient
        );
        lua.new_usertype<RigidBodyComponent>("RigidBodyComponent", 
            "setAngularLockAxisFactor", &RigidBodyComponent::setAngularLockAxisFactor,
            "updateMassProperties", &RigidBodyComponent::updateMassProperties,
            "setMass", &RigidBodyComponent::setMass,
            "getMass", &RigidBodyComponent::getMass
        );

        lua.new_usertype<RaycastHit>("RaycastHit",
            "point", &RaycastHit::point,
            "normal", &RaycastHit::normal,
            "hitObject", &RaycastHit::hitObject,
            "hit", &RaycastHit::hit
        );

        lua.new_usertype<PhysicsEngine>("PhysicsEngine",
            "raycast", &PhysicsEngine::raycast
        );
    }
};