#pragma once

class PhysicsSystem {
public:
    rp3d::PhysicsCommon physicsCommon;
    
    PhysicsSystem() {
        world = physicsCommon.createPhysicsWorld();
    }

    ~PhysicsSystem() {
        physicsCommon.destroyPhysicsWorld(world);
    }

    void update() {
        world->update(1 / 60.0f);
    }

    rp3d::PhysicsWorld* getWorld() { return world; }
private:
    rp3d::PhysicsWorld* world;
};