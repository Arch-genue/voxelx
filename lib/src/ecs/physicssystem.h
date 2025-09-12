#pragma once

class PhysicsSystem {
public:
    rp3d::PhysicsCommon physicsCommon;
    
    PhysicsSystem() {
        world = physicsCommon.createPhysicsWorld();
        world->setGravity(rp3d::Vector3(0, -19.81f, 0));
    }

    ~PhysicsSystem() {
        physicsCommon.destroyPhysicsWorld(world);
    }
    

    void update(float deltaTime) {
        world->update(1 / 60.0f);
    }

    rp3d::PhysicsWorld* getWorld() { return world; }
private:
    rp3d::PhysicsWorld* world;
};