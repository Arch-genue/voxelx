#include "gameobject.h"

#include <GL/glew.h>
#include <glm/ext.hpp>
#include "../graphics/mesh.h"
#include "../graphics/shader.h"
#include "../voxels/voxel.h"
#include "../loaders/resourcemanager.h"
#include "../graphics/renderer.h"


GameObject::GameObject(const char* model) : TransformObject() {
	_campos = glm::vec3(0);

    setVisible(true);

	_voxelmodel = ResourceManager::getModel(model);
	
	BoundingBox boundingbox = {0, 0, 0, 0, 0, 0};
	BoxCollider* collider = new BoxCollider(boundingbox);

	_physicsobject = new PhysicsObject(this, collider, 10.0f);
	setPosition(glm::vec3(0));

    // !CREATE MESH!
	_mesh = Renderer::render(_voxelmodel);
}
GameObject::GameObject(const char *model, glm::vec3 position) : TransformObject() {
	_campos = glm::vec3(0);

	// std::cout << "position: " << position.x << ", " << position.y << ", " << position.z << std::endl;

    setVisible(true);

	_voxelmodel = ResourceManager::getModel(model);
	
	BoundingBox boundingbox = {0, 0, 0, 0, 0, 0};
	BoxCollider* collider = new BoxCollider(boundingbox);

	_physicsobject = new PhysicsObject(this, collider, 10.0f);
	setPosition(position);

    // !CREATE MESH!
	_mesh = Renderer::render(_voxelmodel);
}
GameObject::~GameObject() {}

void GameObject::setGameManager(GameManager* gamemanager) {
	_gm = gamemanager;
}
GameManager* GameObject::getGameManager() {
	return _gm;
}

void GameObject::attachCamera(Camera* cam, glm::vec3 stdpos) {
	_camera = cam;
	_campos = stdpos;
	_camera->setPosition(getPosition() + _campos);
}

PhysicsObject* GameObject::getPhysicsObject() {
	return _physicsobject;
}

void GameObject::setPosition(glm::vec3 position) {
	TransformObject::setPosition(position);
	_physicsobject->setPosition(position);
}

void GameObject::draw() {
	if (isVisible()) {
		ResourceManager::getShader("voxel")->uniformMatrix("model", getMatrix());
		_mesh->draw(GL_TRIANGLES);
		// _boundingboxmesh->draw(GL_LINES);
	}
};

void GameObject::setPhysics(PHYSICS physics) {
	switch(physics) {
		case NO_PHYSICS:
			_physicsobject->getCollider()->setSize(glm::vec3(0, 0, 0)); 
			_physicsobject->setPhysics(physics);
		break;
		case STATIC_PHYSICS:
		case DYNAMIC_PHYSICS:
			glm::vec3 sizes = _voxelmodel->getSize();
			_physicsobject->getCollider()->setSize(glm::vec3(sizes.x, sizes.y, sizes.z));
			// float* vertices = _physicsobject->getVertices();
			// _collider->getBoundingbox()->
			// _bbox.min = getPosition()-0.5f;
			// _bbox.max = _boundbox_size-0.5f;
			
			// int attrs[2] = { 3,  0 };
			// _boundingboxmesh = new Mesh(vertices, 24, attrs);
			// _physobject = new PhysicsObject(getPosition(), , mass);
			
			// _physicsobject->setMass(_mass);
			_physicsobject->setPhysics(physics);
		break;
		// default:
		// 	_physicsobject->getCollider()->setSize(glm::vec3(0, 0, 0));
		// break;
	}
}

// TODO SIMPLE COLLISION
// glm::ivec3 GameObject::checkCollision(BOUNDINGBOX b) {
// 	glm::ivec3 collisionside(1);
// 	if (_bbox.max.x < b.min.x || _bbox.min.x > b.max.x) collisionside.x = 0;

// 	if (_bbox.max.y < b.min.y || _bbox.min.y > b.max.y) { 
// 		collisionside.y = 0; _onGround = false; 
// 	} else 
// 		_onGround = true;

// 	if (_bbox.max.z < b.min.z || _bbox.min.z > b.max.z) collisionside.z = 0;
// 	return collisionside;
// }
// bool GameObject::checkGround() {
// 	return _onGround;
// }

// BOUNDINGBOX GameObject::getBBOX() {
// 	return _bbox;
// }




VoxelModel* GameObject::getVoxelModel() {
	return _voxelmodel;
    // if (_visible == false) return false;

	// VoxelModel* voxels = _mesh->getVoxels();
	// glm::vec3 halfSize = glm::vec3(0.1f) * 0.5f;
	// for (int i = 0; i < voxels->getVoxelsCount(); i++) {
	// 	Voxel* voxel = voxels->getVoxel(i);
	// 	glm::vec3 position = voxel->getPosition();
	// 	//std::cout << voxs->voxels[i].position.x << " " << voxs->voxels[i].position.y << " " << voxs->voxels[i].position.z << std::endl;
	// 	// if (voxs->voxels[i].position == pos) {
	// 	// 	return &voxs->voxels[i];
	// 	// }
	// 	glm::vec3 voxelMin = position - halfSize;
    // 	glm::vec3 voxelMax = position + halfSize;
	// 	if (point.x >= voxelMin.x && point.x <= voxelMax.x &&
    //         point.y >= voxelMin.y && point.y <= voxelMax.y &&
    //         point.z >= voxelMin.z && point.z <= voxelMax.z) return true;
	// }
	// return false;
}

void GameObject::setMesh(Mesh* mesh) {
	_mesh = mesh;
}

Mesh* GameObject::getMesh() {
	return _mesh;
}

bool GameObject::raycast(glm::vec3 pos, glm::vec3 dir, float maxDist, glm::vec3& end, glm::vec3& norm, glm::vec3& iend) {
    if (!isVisible()) return false;

	float px = pos.x;
	float py = pos.y;
	float pz = pos.z;

	float dx = dir.x;
	float dy = dir.y;
	float dz = dir.z;

	float t = 0.0f;
	int ix = floor(px);
	int iy = floor(py);
	int iz = floor(pz);

	float stepx = (dx > 0.0f) ? 1.0f : -1.0f;
	float stepy = (dy > 0.0f) ? 1.0f : -1.0f;
	float stepz = (dz > 0.0f) ? 1.0f : -1.0f;

	float infinity = std::numeric_limits<float>::infinity();

	float txDelta = (dx == 0.0f) ? infinity : abs(1.0f / dx);
	float tyDelta = (dy == 0.0f) ? infinity : abs(1.0f / dy);
	float tzDelta = (dz == 0.0f) ? infinity : abs(1.0f / dz);

	float xdist = (stepx > 0) ? (ix + 1 - px) : (px - ix);
	float ydist = (stepy > 0) ? (iy + 1 - py) : (py - iy);
	float zdist = (stepz > 0) ? (iz + 1 - pz) : (pz - iz);

	float txMax = (txDelta < infinity) ? txDelta * xdist : infinity;
	float tyMax = (tyDelta < infinity) ? tyDelta * ydist : infinity;
	float tzMax = (tzDelta < infinity) ? tzDelta * zdist : infinity;

	int steppedIndex = -1;
    

	while (t <= maxDist) {
		//voxel* voxel = get(ix, iy, iz);
		//if (voxel == nullptr || voxel->id){
        // if (getVoxel(glm::vec3(ix, iy, iz))) {
		// 	end.x = px + t * dx;
		// 	end.y = py + t * dy;
		// 	end.z = pz + t * dz;

		// 	iend.x = ix;
		// 	iend.y = iy;
		// 	iend.z = iz;

		// 	norm.x = norm.y = norm.z = 0.0f;
		// 	if (steppedIndex == 0) norm.x = -stepx;
		// 	if (steppedIndex == 1) norm.y = -stepy;
		// 	if (steppedIndex == 2) norm.z = -stepz;

        //     return true;
		// }
		if (txMax < tyMax) {
			if (txMax < tzMax) {
				ix += stepx;
				t = txMax;
				txMax += txDelta;
				steppedIndex = 0;
			} else {
				iz += stepz;
				t = tzMax;
				tzMax += tzDelta;
				steppedIndex = 2;
			}
		} else {
			if (tyMax < tzMax) {
				iy += stepy;
				t = tyMax;
				tyMax += tyDelta;
				steppedIndex = 1;
			} else {
				iz += stepz;
				t = tzMax;
				tzMax += tzDelta;
				steppedIndex = 2;
			}
		}
	}
	iend.x = ix;
	iend.y = iy;
	iend.z = iz;

	end.x = px + t * dx;
	end.y = py + t * dy;
	end.z = pz + t * dz;
	norm.x = norm.y = norm.z = 0.0f;
	return false;
}