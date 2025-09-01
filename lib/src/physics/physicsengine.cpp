#include "physicsengine.h"
#include "../voxels/voxel.h"

class VoxelModel;

// class MyCallbackClass : public rp3d::RaycastCallback { 
// private:
//     // GameObject* _gameobj;
// public: 
//     void setCurrentObject(GameObject *obj) {
//         _gameobj = obj;
//     }
//     virtual rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info) { 
//         // GameObject* userData = static_cast<GameObject*>(info.collider->getUserData());

//         if (userData != nullptr) {
//             // if (_gameobj != nullptr) {
//             //     _gameobj->getPhysicsObject()->setIsGround(true);
//             // }
//         }
//         return rp3d::decimal(1.0); 
//     } 
// };
static bool test = false;
#include <chrono>

struct PhysicsRaycastCallback : public rp3d::RaycastCallback {
public:
    RaycastHit result;
    bool hit = false;
    // Указатель на первый столкнувшийся объект
    const rp3d::RaycastInfo* hitInfo = nullptr;

    float notifyRaycastHit(const rp3d::RaycastInfo& info) override {
        if (!hitInfo) hitInfo = &info;
        GameObject* userData = static_cast<GameObject*>(info.collider->getUserData());

        if (userData != nullptr) {
            result.point = glm::vec3(info.worldPoint.x, info.worldPoint.y, info.worldPoint.z);
            result.normal = glm::vec3(info.worldNormal.x, info.worldNormal.y, info.worldNormal.z);
            result.hitObject = userData;

            // raycastInfo.body.
            hit = true;
            return info.hitFraction; // останавливаем на первом попадании
        }
    }
};

class SpecialCallback : public rp3d::RaycastCallback {
public:
    // Указатель на первый столкнувшийся объект
    const rp3d::RaycastInfo* hitInfo = nullptr;

    // Этот метод вызывается для каждого пересечения
    float notifyRaycastHit(const rp3d::RaycastInfo& info) override {
        // // Сохраняем данные первого столкновения
        // if (!hitInfo) hitInfo = &info;
        // GameObject* userData = static_cast<GameObject*>(info.collider->getUserData());
        // rp3d::Vector3 hitPoint = info.worldPoint;

        // // if (test) {
        // //     return 0.0f;
        // // }

        // if (userData != nullptr) {
        //     int vx = static_cast<int>(std::floor(hitPoint.x));
        //     int vy = static_cast<int>(std::floor(hitPoint.y));
        //     int vz = static_cast<int>(std::floor(hitPoint.z));
            
        //     int radius = 10;
            
        //     auto start = std::chrono::high_resolution_clock::now();
        //     VoxelModel* voxmodel = userData->getMeshModel()->getVoxelModel();
        //     for (int x = vx - radius; x <= vx + radius; ++x) {
        //         for (int y = vy - radius; y <= vy + radius; ++y) {
        //             for (int z = vz - radius; z <= vz + radius; ++z) {
        //                 int dx = x - vx;
        //                 int dy = y - vy;
        //                 int dz = z - vz;
        //                 if (dx*dx + dy*dy + dz*dz <= radius*radius) {
        //                     Voxel& voxel = voxmodel->voxel(x, y-50, z);
        //                     voxel.color = glm::vec4(1.0f, 0.0f, 0.0f, 0.7f);
        //                     voxel.visible = true;
        //                 }
        //             }
        //         }
        //     }
        //     auto end = std::chrono::high_resolution_clock::now();
        //     // разница в микросекундах
        //     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        //     std::cout << "Time elapsed: " << duration << " ms\n";

        //     std::cout << "Explode!" << "\n";
        //     test = true;
        // }

        // return 0.0f;
    }
};

// Your event listener class 
class YourEventListener : public rp3d::EventListener { 
    // Override the onContact() method 
    virtual void onContact(const rp3d::CollisionCallback::CallbackData& callbackData) override { 
        // For each contact pair 
        for (uint p = 0; p < callbackData.getNbContactPairs(); p++) { 
            // Get the contact pair 
            rp3d::CollisionCallback::ContactPair contactPair = callbackData.getContactPair(p); 
        
            // For each contact point of the contact pair 
            for (uint c = 0; c < contactPair.getNbContactPoints(); c++) { 
        
                // Get the contact point 
                rp3d::CollisionCallback::ContactPoint contactPoint = contactPair.getContactPoint(c); 
        
                // Get the contact point on the first collider and convert it in world-space 
                rp3d::Vector3 worldPoint = contactPair.getCollider1()->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider1(); 
				// void* test = contactPair.getCollider1()->get
				// std::cout << "worldPoint: " << test->getID() << std::endl;
            } 
        }
    } 
}; 

PhysicsEngine::PhysicsEngine() {    
    rp3d::PhysicsWorld::WorldSettings settings; 
    settings.defaultVelocitySolverNbIterations = 20; 
    settings.isSleepingEnabled = false; 
    settings.gravity = rp3d::Vector3(0, -9.81, 0);
 
    _world = _physicsCommon.createPhysicsWorld(settings);
    _world->setIsDebugRenderingEnabled(true);

    rp3d::DebugRenderer& debugRenderer = _world->getDebugRenderer(); 
    
    debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_POINT, true); 
    debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_NORMAL, true);
    debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);

    Logger::instance().log(LogLevel::INFO, "PHYSICS", "PhysicsEngine initialized");
}

rp3d::PhysicsCommon &PhysicsEngine::getPhysicsCommon() {
    return _physicsCommon;
}

rp3d::PhysicsWorld *PhysicsEngine::getPhysicsWorld() {
    return _world;
}

RaycastHit PhysicsEngine::raycast(const glm::vec3& origin, const glm::vec3& dir) {
    reactphysics3d::Ray ray(
        reactphysics3d::Vector3(origin.x, origin.y, origin.z),
        reactphysics3d::Vector3(dir.x, dir.y, dir.z) // нормализуем
    );

    // std::cout << "Raycasted: " << origin.y << " " << dir.y << "\n";

    PhysicsRaycastCallback callback;

    _world->raycast(ray, &callback);

    RaycastHit result{};
    if (callback.hit) {
        result.hit = true;
        result = callback.result;
    } else {
        result.hit = false;
        result.hitObject = nullptr;
    }
    return result;
}

rp3d::RigidBody* PhysicsEngine::createRigidBody(glm::vec3 position) {
    return _world->createRigidBody(rp3d::Transform(rp3d::Vector3(position.x, position.y, position.z), rp3d::Quaternion::identity()));
}

void PhysicsEngine::update(float deltaTime) {
	_world->update(deltaTime);

    for (auto& object : _objects) {
        object->update(deltaTime);
    }
}