#include "physicsengine.h"
#include "../voxels/voxel.h"

class GameObject;
class VoxelModel;

PhysicsEngine::PhysicsEngine() {
    _gravity = glm::vec3(0, -9.81f, 0);
	// _root = nullptr;

    Logger::eprint("PHYSENG", "PhysicsEngine initialized",  LOGLEVEL::INFO);
}
PhysicsEngine::~PhysicsEngine() {}

glm::vec3 PhysicsEngine::getGravity() {
    return _gravity;
}

void PhysicsEngine::addObject(PhysicsObject* object) {
    _objects.push_back(object);
	// _root = insert(_root, object->getCollider());
}
PhysicsObject* PhysicsEngine::getObject(int i) {
    return _objects[i];
}

void PhysicsEngine::update(float deltaTime) {
    for (auto& object : _objects) {
        object->update(deltaTime);
    }
}

bool PhysicsEngine::checkCollision(PhysicsObject* object, glm::vec3& surfacePosition, glm::vec3& surfaceNormal) {
    // Проверяем столкновение между объектом и поверхностью
    // В этом примере мы считаем, что поверхность находится на высоте 0
    if (object->getPhysics() == STATIC_PHYSICS or object->getPhysics() == NO_PHYSICS) return false;

    return object->checkGround(surfacePosition, surfaceNormal);
    
    // return false;
}
void PhysicsEngine::handleCollision(PhysicsObject* object, glm::vec3 surfacePosition, glm::vec3 surfaceNormal) {
     // Применяем реакцию на столкновение
    glm::vec3 velocity = object->getVelocity();
    glm::vec3 acceleration = object->getAcceleration();

    // // Изменяем скорость объекта в направлении, перпендикулярном поверхности
    // glm::vec3 normalVelocity = glm::dot(velocity, surfaceNormal) * surfaceNormal;
    // glm::vec3 tangentVelocity = velocity - normalVelocity;
    velocity = glm::vec3(0.0f); //tangentVelocity - 0.8f * normalVelocity;

    // // Изменяем ускорение объекта в направлении, перпендикулярном поверхности
    // glm::vec3 normalAcceleration = glm::dot(acceleration, surfaceNormal) * surfaceNormal;
    // glm::vec3 tangentAcceleration = acceleration - normalAcceleration;
    acceleration = glm::vec3(0.0f);// tangentAcceleration - 0.8f * normalAcceleration;

    // Обновляем скорость и ускорение объекта
    object->setVelocity(velocity);
    object->setAcceleration(acceleration);
    
    GameObject* gmobj = object->getGameObject(); 
    gmobj->setPosition(surfacePosition + surfaceNormal * 0.0f);    
}

// Функция для проверки столкновения луча с BoundingBox
bool PhysicsEngine::raycast(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, PhysicsObject* physicsobject) {
    BoxCollider* box = physicsobject->getCollider();
    // Параметры для проверки пересечения с BoundingBox
    float tMin = (box->getMin().x - rayOrigin.x) / rayDirection.x;
    float tMax = (box->getMax().x - rayOrigin.x) / rayDirection.x;

    float tYMin = (box->getMin().y - rayOrigin.y) / rayDirection.y;
    float tYMax = (box->getMax().y - rayOrigin.y) / rayDirection.y;

    if (tMin > tMax) std::swap(tMin, tMax);
    if (tYMin > tYMax) std::swap(tYMin, tYMax);

    if ((tMin > tYMax) || (tYMin > tMax)) {
        return false; // Нет пересечения с BoundingBox по оси X
    }

    if (tYMin > tMin) {
        tMin = tYMin;
    }

    if (tYMax < tMax) {
        tMax = tYMax;
    }

    float tZMin = (box->getMin().z - rayOrigin.z) / rayDirection.z;
    float tZMax = (box->getMax().z - rayOrigin.z) / rayDirection.z;

    if (tZMin > tZMax) std::swap(tZMin, tZMax);

    if ((tMin > tZMax) || (tZMin > tMax)) {
        return false; // Нет пересечения с BoundingBox по оси Y
    }

    return true; // Произошло пересечение с BoundingBox
}

bool PhysicsEngine::raycast(GameObject* gameobject, glm::vec3 pos, glm::vec3 dir, float maxDist, glm::vec3& end, glm::vec3& norm, glm::vec3& iend) {
    
    
    // float px = pos.x;
	// float py = pos.y;
	// float pz = pos.z;

	// float dx = dir.x;
	// float dy = dir.y;
	// float dz = dir.z;

	// float t = 0.0f;
	// int ix = floor(px);
	// int iy = floor(py);
	// int iz = floor(pz);

	// float stepx = (dx > 0.0f) ? 1.0f : -1.0f;
	// float stepy = (dy > 0.0f) ? 1.0f : -1.0f;
	// float stepz = (dz > 0.0f) ? 1.0f : -1.0f;

	// float infinity = std::numeric_limits<float>::infinity();

	// float txDelta = (dx == 0.0f) ? infinity : abs(1.0f / dx);
	// float tyDelta = (dy == 0.0f) ? infinity : abs(1.0f / dy);
	// float tzDelta = (dz == 0.0f) ? infinity : abs(1.0f / dz);

	// float xdist = (stepx > 0) ? (ix + 1 - px) : (px - ix);
	// float ydist = (stepy > 0) ? (iy + 1 - py) : (py - iy);
	// float zdist = (stepz > 0) ? (iz + 1 - pz) : (pz - iz);

	// float txMax = (txDelta < infinity) ? txDelta * xdist : infinity;
	// float tyMax = (tyDelta < infinity) ? tyDelta * ydist : infinity;
	// float tzMax = (tzDelta < infinity) ? tzDelta * zdist : infinity;

	// int steppedIndex = -1;

	// while (t <= maxDist) {
	// 	// Voxel* voxel = get(ix, iy, iz);
	// 	// if (voxel == nullptr || voxel->id) return;
    //     Voxel* voxel = gameobject->getVoxelModel()->getVoxel(glm::vec3(ix, iy, iz));
    //     // VoxelModel* getVoxel(glm::vec3(ix, iy, iz));
    //     if (voxel) {
	// 		end.x = px + t * dx;
	// 		end.y = py + t * dy;
	// 		end.z = pz + t * dz;

	// 		iend.x = ix;
	// 		iend.y = iy;
	// 		iend.z = iz;

	// 		norm.x = norm.y = norm.z = 0.0f;
	// 		if (steppedIndex == 0) norm.x = -stepx;
	// 		if (steppedIndex == 1) norm.y = -stepy;
	// 		if (steppedIndex == 2) norm.z = -stepz;

    //         return true;
	// 	}
	// 	if (txMax < tyMax) {
	// 		if (txMax < tzMax) {
	// 			ix += stepx;
	// 			t = txMax;
	// 			txMax += txDelta;
	// 			steppedIndex = 0;
	// 		} else {
	// 			iz += stepz;
	// 			t = tzMax;
	// 			tzMax += tzDelta;
	// 			steppedIndex = 2;
	// 		}
	// 	} else {
	// 		if (tyMax < tzMax) {
	// 			iy += stepy;
	// 			t = tyMax;
	// 			tyMax += tyDelta;
	// 			steppedIndex = 1;
	// 		} else {
	// 			iz += stepz;
	// 			t = tzMax;
	// 			tzMax += tzDelta;
	// 			steppedIndex = 2;
	// 		}
	// 	}
	// }
	// iend.x = ix;
	// iend.y = iy;
	// iend.z = iz;

	// end.x = px + t * dx;
	// end.y = py + t * dy;
	// end.z = pz + t * dz;
	// norm.x = norm.y = norm.z = 0.0f;
	return false;
}