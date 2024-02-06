#include "gameobject.h"

#include <GL/glew.h>
#include <glm/ext.hpp>
#include "../graphics/mesh.h"
#include "../graphics/shader.h"
#include "../voxels/voxel.h"
#include "../loaders/resourcemanager.h"
#include "../graphics/renderer.h"

#include <iostream>

GameObject::GameObject(const char* model) {
    modelmatrix = glm::mat4(1.0f);
    position = glm::vec3(0);
    rotAngle = 0.0f;
    rotAxis = glm::vec3(1);
    scaling = glm::vec3(0.1f);
	campos = glm::vec3(0);

    setVisible(true);
	setHidden(false);

	_physobject = new PhysicsObject(this);
	onGround = false;

    mass = 10; // 100 Kg

	voxels = ResourceManager::getModel(model);

    // !CREATE MESH!
	mesh = Renderer::render(voxels);
}
GameObject::~GameObject() {}

void GameObject::setGameManager(GameManager* gamemanager) {
	gm = gamemanager;
}

void GameObject::setLight(glm::vec3 *light) {
	for (uint16_t i = 0; i < 6; i++) voxels->light[i] = light[i];
}
glm::vec3* GameObject::getLight() {
	return voxels->light;
}

void GameObject::attachCamera(Camera* cam, vec3 stdpos) {
	camera = cam;
	campos = stdpos;
	camera->position = getPosition() + campos;
}

void GameObject::draw() {
    if (visible == false) return;
    modelmatrix = glm::scale(modelmatrix, scaling);
    modelmatrix = glm::translate(modelmatrix, position);
    ResourceManager::getShader("voxel")->uniformMatrix("model", modelmatrix);
    if (!hidden && visible) mesh->draw(GL_TRIANGLES);

    modelmatrix = glm::mat4(1.0f);
}

PhysicsObject* GameObject::getPhysics() {
	return _physobject;
}

void GameObject::setCollision(_collision coll) {
	switch(coll) {
		case NO_COLLISION: 
			_boundbox_size = glm::vec3(0, 0, 0); 
		break;
		case SIMPLE_COLLISION:
			glm::vec3 sizes = mesh->getVoxels()->m_size;
			_boundbox_size = glm::vec3(sizes.x, sizes.y, sizes.z);
			bbox.min = getPosition()-0.5f;
			bbox.max = _boundbox_size-0.5f;
			// float vertices[] = {
			// 	//x     y
			// 	getPosition().x-0.5f, -0.5f, -0.5f,
			// 	getPosition().x-0.5f, _boundbox_size.y-0.5f, -0.5f,

				
			// 	_boundbox_size.x-0.5f, _boundbox_size.y-0.5f, -0.5f,
			// 	_boundbox_size.x-0.5f, -0.5f, -0.5f,

			// 	getPosition().x-0.5f, -0.5f, _boundbox_size.z-0.5f,
			// 	getPosition().x-0.5f, _boundbox_size.y-0.5f, _boundbox_size.z-0.5f,

				
			// 	_boundbox_size.x-0.5f, _boundbox_size.y-0.5f, _boundbox_size.z-0.5f,
			// 	_boundbox_size.x-0.5f, -0.5f, _boundbox_size.z-0.5f,
			// };
			// int attrs[2] = { 3,  0 };
			//_boundingbox = new Mesh(vertices, 8, attrs);
			//_physobject = new PhysicsObject(getPosition(), , mass);
			_physobject->setType(getRigidBody() ? DYNAMIC_PHYSICS : STATIC_PHYSICS);
			_physobject->setMass(mass);
		break;
	}
	collision = coll;
}

bool GameObject::getCollision() {
	return (collision == SIMPLE_COLLISION);
}

void GameObject::setRigidBody(bool rigid) {
	rigidbody = rigid;
}

bool GameObject::getRigidBody() {
	return rigidbody;
}

//TODO SIMPLE COLLISION
glm::ivec3 GameObject::checkCollision(BOUNDINGBOX b) {
	glm::ivec3 collisionside(1);
	if (bbox.max.x < b.min.x || bbox.min.x > b.max.x) collisionside.x = 0;
	if (bbox.max.y < b.min.y || bbox.min.y > b.max.y) { collisionside.y = 0; onGround = false; } else onGround = true;
	if (bbox.max.z < b.min.z || bbox.min.z > b.max.z) collisionside.z = 0;
	return collisionside;
}
bool GameObject::checkGround() {
	return onGround;
}

BOUNDINGBOX GameObject::getBBOX() {
	return bbox;
}

void GameObject::translate(float val, glm::vec3 vec) {
	position += vec*val;
}

void GameObject::rotate(float angle, glm::vec3 rot) {
    rotAngle += angle;
    rotAxis = rot; 
}

void GameObject::setPosition(glm::vec3 pos) {
	position = pos;
	_physobject->setPosition(pos);
}
void GameObject::setRotation(float angle, glm::vec3 rot) {
    rotAngle = angle;
    rotAxis = rot; 
    glm::mat4 rotation = glm::rotate(modelmatrix, rotAngle, rotAxis);

    modelmatrix = rotation * modelmatrix;
}
void GameObject::setVisible(bool vis) {
    visible = vis;
}
void GameObject::setHidden(bool hid) {
    hidden = hid;
}

glm::vec3 GameObject::getPosition() {
    return position;
}
glm::mat4 GameObject::getMatrix() {
    return modelmatrix;
}
bool GameObject::getVoxel(glm::vec3 point) {
    if (visible == false) return false;

	_voxels* voxs = mesh->getVoxels();
	glm::vec3 halfSize = glm::vec3(0.1f) * 0.5f;
	for (size_t i = 0; i < voxs->voxels.size(); i++) {
		//std::cout << voxs->voxels[i].position.x << " " << voxs->voxels[i].position.y << " " << voxs->voxels[i].position.z << std::endl;
		// if (voxs->voxels[i].position == pos) {
		// 	return &voxs->voxels[i];
		// }
		glm::vec3 voxelMin = voxs->voxels[i].position - halfSize;
    	glm::vec3 voxelMax = voxs->voxels[i].position + halfSize;
		if (point.x >= voxelMin.x && point.x <= voxelMax.x &&
            point.y >= voxelMin.y && point.y <= voxelMax.y &&
            point.z >= voxelMin.z && point.z <= voxelMax.z) return true;
	}
	return false;
}

void GameObject::setMesh(Mesh* newmesh) {
	mesh = newmesh;
}

Mesh* GameObject::getMesh() {
	return mesh;
}

bool GameObject::raycast(glm::vec3 pos, glm::vec3 dir, float maxDist, glm::vec3& end, glm::vec3& norm, glm::vec3& iend) {
    if (visible == false) return false;

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
        if (getVoxel(glm::vec3(ix, iy, iz))) {
			end.x = px + t * dx;
			end.y = py + t * dy;
			end.z = pz + t * dz;

			iend.x = ix;
			iend.y = iy;
			iend.z = iz;

			norm.x = norm.y = norm.z = 0.0f;
			if (steppedIndex == 0) norm.x = -stepx;
			if (steppedIndex == 1) norm.y = -stepy;
			if (steppedIndex == 2) norm.z = -stepz;

            return true;
		}
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

GameManager* GameObject::getGameManager() {
	return gm;
}