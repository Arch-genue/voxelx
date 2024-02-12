#include "physicsengine.h"
#include "../voxels/voxel.h"

class GameObject;
class VoxelModel;

PhysicsEngine::PhysicsEngine() {
    _gravity = glm::vec3(0, -9.81f, 0);
	_root = nullptr;
}
PhysicsEngine::~PhysicsEngine() {}

glm::vec3 PhysicsEngine::getGravity() {
    return _gravity;
}

void PhysicsEngine::addObject(PhysicsObject* object) {
    _objects.push_back(object);
	_root = insert(_root, object->getCollider());
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

// Функция для выполнения левого поворота в красно-черном дереве
Node* PhysicsEngine::rotateLeft(Node* root) {
    Node* newRoot = root->getRight();
    root->setRight(newRoot->getLeft());
    newRoot->setLeft(root);
    newRoot->setIsRed(root->isRed());
    root->setIsRed(true);
    return newRoot;
}

// Функция для выполнения правого поворота в красно-черном дереве
Node* PhysicsEngine::rotateRight(Node* root) {
    Node* newRoot = root->getLeft();
    root->setLeft(newRoot->getRight());
    newRoot->setRight(root);
    newRoot->setIsRed(root->isRed());
    root->setIsRed(true);
    return newRoot;
}

// Функция для перекрашивания узлов в красно-черном дереве
void PhysicsEngine::flipColors(Node* root) {
    root->setIsRed(!root->isRed());
    root->getLeft()->setIsRed(!root->getLeft()->isRed());
    root->getRight()->setIsRed(!root->getRight()->isRed());
}

// Функция для добавления узла в красно-черное дерево
Node* PhysicsEngine::insert(Node* root, BoxCollider* box) {
    if (!root) {
		// std::cout << "root is null" << std::endl;
        return new Node(box);
    }

    // Рекурсивное добавление узла
    if (box->getMin().x < root->getBoxCollider()->getMin().x) {
        root->setLeft(insert(root->getLeft(), box));
    } else {
        root->setRight(insert(root->getRight(), box));
    }

    // Балансировка дерева
    if (root->getRight() && root->getRight()->isRed() && !(root->getLeft() && root->getLeft()->isRed())) {
        root = rotateLeft(root);
    }
    if (root->getLeft() && root->getLeft()->isRed() && root->getLeft()->getLeft() && root->getLeft()->getLeft()->isRed()) {
        root = rotateRight(root);
    }
    if (root->getLeft() && root->getLeft()->isRed() && root->getRight() && root->getRight()->isRed()) {
        flipColors(root);
    }

    return root;
}

// Функция для поиска ближайшего объекта в дереве AABB
Node* PhysicsEngine::findNearestObject(Node* root, BoxCollider* area) {
    if (!root) {
        return nullptr;
    }

	// std::cout << "Root bounding box: " << root->getBoxCollider()->getMin().x << ", " << root->getBoxCollider()->getMin().y << ", " << root->getBoxCollider()->getMin().z << std::endl;
	// std::cout << "Area bounding box: " << area->getMin().x << ", " << area->getMin().y << ", " << area->getMin().z << std::endl;
	// a.minX <= b.maxX &&
    // a.maxX >= b.minX &&
    // a.minY <= b.maxY &&
    // a.maxY >= b.minY &&
    // a.minZ <= b.maxZ &&
    // a.maxZ >= b.minZ
	// std::cout << root->getBoxCollider()->getGameObject()->getID() << std::endl;

	if (root->getBoxCollider()->getMin().x <= area->getMax().x && root->getBoxCollider()->getMax().x >= area->getMin().x &&
	   	root->getBoxCollider()->getMin().y <= area->getMax().y && root->getBoxCollider()->getMax().y >= area->getMin().y &&
		root->getBoxCollider()->getMin().z <= area->getMax().z && root->getBoxCollider()->getMax().z >= area->getMin().z
	   ) {
		// std::cout << "Intersect";
	} else {
		return nullptr;
	}

    // Если текущий узел пересекается с заданной областью, проверяем его дочерние узлы
    Node* nearestLeft = findNearestObject(root->getLeft(), area);
    Node* nearestRight = findNearestObject(root->getRight(), area);

    // Выбираем ближайший узел из дочерних
    Node* nearest = nearestLeft ? nearestLeft : nearestRight;

    // Проверяем, нужно ли заменить ближайший узел на текущий
    if (nearest) {
        float nearestDist = (nearest->getBoxCollider()->getMin().x - area->getMax().x) * (nearest->getBoxCollider()->getMin().x - area->getMax().x) +
                            (nearest->getBoxCollider()->getMin().y - area->getMax().y) * (nearest->getBoxCollider()->getMin().y - area->getMax().y) +
                            (nearest->getBoxCollider()->getMin().z - area->getMax().z) * (nearest->getBoxCollider()->getMin().z - area->getMax().z);

        float rootDist = (root->getBoxCollider()->getMin().x - area->getMax().x) * (root->getBoxCollider()->getMin().x - area->getMax().x) +
                         (root->getBoxCollider()->getMin().y - area->getMax().y) * (root->getBoxCollider()->getMin().y - area->getMax().y) +
                         (root->getBoxCollider()->getMin().z - area->getMax().z) * (root->getBoxCollider()->getMin().z - area->getMax().z);

        if (rootDist < nearestDist) {
            nearest = root;
        }
    } else {
        nearest = root;
    }

    return nearest;
}

Node *PhysicsEngine::getRootNode() {
    return _root;
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