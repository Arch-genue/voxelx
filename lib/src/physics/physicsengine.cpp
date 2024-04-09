#include "physicsengine.h"
#include "../voxels/voxel.h"

class GameObject;
class VoxelModel;

class MyCallbackClass : public rp3d::RaycastCallback { 
private:
    GameObject* _gameobj;
public: 
    void setCurrentObject(GameObject *obj) {
        _gameobj = obj;
    }
    virtual rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info) { 
        GameObject* userData = static_cast<GameObject*>(info.collider->getUserData());

        if (userData != nullptr) {
            if (_gameobj != nullptr) {
                _gameobj->getPhysicsObject()->setIsGround(true);
            }
        }
        return rp3d::decimal(1.0); 
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

    vLogger::eprint("PHYSICS", "PhysicsEngine initialized",  LOGLEVEL::INFO);
}
PhysicsEngine::~PhysicsEngine() {}

rp3d::PhysicsCommon &PhysicsEngine::getPhysicsCommon() {
    return _physicsCommon;
}

rp3d::PhysicsWorld *PhysicsEngine::getPhysicsWorld() {
    return _world;
}

PhysicsObject *PhysicsEngine::createRigidBody(GameObject *gmobj, glm::vec3 size) {
    rp3d::Vector3 position(gmobj->getPosition().x, gmobj->getPosition().y, gmobj->getPosition().z);
	rp3d::RigidBody* rigidbody = _world->createRigidBody(rp3d::Transform(position, rp3d::Quaternion::identity()));
    rigidbody->setMass(1.0f);
    
    glm::vec3 cnt(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f);
	
	const rp3d::Vector3 halfExtents (cnt.x, cnt.y, cnt.z);  
	rp3d::BoxShape* shape = _physicsCommon.createBoxShape(halfExtents);

	rp3d::Collider* collider = rigidbody->addCollider(shape, rp3d::Transform::identity());

    rp3d::Transform transform;
    transform.identity(); // Установка трансформации в единичную матрицу
    transform.setPosition(rp3d::Vector3(cnt.x-0.5f, cnt.y-0.5f, cnt.z-0.5f)); // Установка позиции в центр куба
    collider->setLocalToBodyTransform(transform); // Установка трансформации для коллайдера

    collider->setUserData(gmobj); // Присваиваем коллайдеру GameObject
	rigidbody->setType(rp3d::BodyType::STATIC); // По умолчанию Статический

    //! TEMPORARY
    rigidbody->setIsDebugEnabled(true);

	rp3d::Material& mat = collider->getMaterial();      
	mat.setBounciness (0); 
	mat.setFrictionCoefficient (20);
	mat.setMassDensity(0);
    
	PhysicsObject* obj = new PhysicsObject(gmobj, rigidbody);
	return obj;
}

void PhysicsEngine::addObject(PhysicsObject* object) {
	// PhysicsObject* obj = createRigidBody(object, object->getCollider(), 1.0f);
    _objects.push_back(object);
}
PhysicsObject* PhysicsEngine::getObject(int i) {
    return _objects[i];
}

void PhysicsEngine::update(float deltaTime) {
	_world->update(1.0f / 60.0f);
    MyCallbackClass obj;

    for (auto& object : _objects) {
        object->update(deltaTime);
        // Получите нижнюю точку вашего объекта (например, его позицию или центр массы)
        glm::vec3 curPos = object->getGameObject()->getPosition();
        // Start and end points of the ray 
        rp3d::Vector3 startPoint = rp3d::Vector3(curPos.x, curPos.y, curPos.z);
        rp3d::Vector3 endPoint = rp3d::Vector3(curPos.x, curPos.y - 1.0f, curPos.z);
        obj.setCurrentObject(object->getGameObject());
        
        rp3d::Ray ray(startPoint, endPoint);
        object->setIsGround(false);
        _world->raycast(ray, &obj);
    }
}