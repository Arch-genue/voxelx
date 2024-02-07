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
    _modelmatrix = glm::mat4(1.0f);
    _position = glm::vec3(0);
    _rotAngle = 0.0f;
    _rotAxis = glm::vec3(1);
    _scaling = glm::vec3(0.1f);
	_campos = glm::vec3(0);

    setVisible(true);
	setHidden(false);

	_physobject = new PhysicsObject(this);
	_onGround = false;

    _mass = 10; // 100 Kg

	_voxelmodel = ResourceManager::getModel(model);

    // !CREATE MESH!
	_mesh = Renderer::render(_voxelmodel);
}
GameObject::~GameObject() {}

void GameObject::setGameManager(GameManager* gamemanager) {
	_gm = gamemanager;
}

void GameObject::setLight(glm::vec3 *light) {
	for (uint16_t i = 0; i < 6; i++) _voxelmodel->setLight(i, light[i]);
}
glm::vec3* GameObject::getLight() {
	return _voxelmodel->getLightArray();
}

void GameObject::attachCamera(Camera* cam, glm::vec3 stdpos) {
	_camera = cam;
	_campos = stdpos;
	_camera->setPosition(getPosition() + _campos);
}

void GameObject::draw() {
    if (_visible == false) return;
    _modelmatrix = glm::scale(_modelmatrix, _scaling);
    _modelmatrix = glm::translate(_modelmatrix, _position);
    ResourceManager::getShader("voxel")->uniformMatrix("model", _modelmatrix);
    if (!_hidden && _visible) _mesh->draw(GL_TRIANGLES);

    _modelmatrix = glm::mat4(1.0f);
}

PhysicsObject* GameObject::getPhysics() {
	return _physobject;
}

void GameObject::setCollision(Collision coll) {
	switch(coll) {
		case NO_COLLISION: 
			_boundbox_size = glm::vec3(0, 0, 0); 
		break;
		case SIMPLE_COLLISION:
			glm::vec3 sizes = _voxelmodel->getSize();
			_boundbox_size = glm::vec3(sizes.x, sizes.y, sizes.z);
			_bbox.min = getPosition()-0.5f;
			_bbox.max = _boundbox_size-0.5f;
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
			_physobject->setMass(_mass);
		break;
	}
	_collision = coll;
}

bool GameObject::getCollision() {
	return (_collision == SIMPLE_COLLISION);
}

void GameObject::setRigidBody(bool rigid) {
	_rigidbody = rigid;
}

bool GameObject::getRigidBody() {
	return _rigidbody;
}

//TODO SIMPLE COLLISION
glm::ivec3 GameObject::checkCollision(BOUNDINGBOX b) {
	glm::ivec3 collisionside(1);
	if (_bbox.max.x < b.min.x || _bbox.min.x > b.max.x) collisionside.x = 0;

	if (_bbox.max.y < b.min.y || _bbox.min.y > b.max.y) { 
		collisionside.y = 0; _onGround = false; 
	} else 
		_onGround = true;

	if (_bbox.max.z < b.min.z || _bbox.min.z > b.max.z) collisionside.z = 0;
	return collisionside;
}
bool GameObject::checkGround() {
	return _onGround;
}

BOUNDINGBOX GameObject::getBBOX() {
	return _bbox;
}

void GameObject::translate(float val, glm::vec3 vec) {
	_position += vec*val;
}

void GameObject::rotate(float angle, glm::vec3 rot) {
    _rotAngle += angle;
    _rotAxis = rot; 
}

void GameObject::setPosition(glm::vec3 pos) {
	_position = pos;
	_physobject->setPosition(pos);
}
void GameObject::setRotation(float angle, glm::vec3 rot) {
    _rotAngle = angle;
    _rotAxis = rot; 
    glm::mat4 rotation = glm::rotate(_modelmatrix, _rotAngle, _rotAxis);

    _modelmatrix = rotation * _modelmatrix;
}
void GameObject::setVisible(bool vis) {
    _visible = vis;
}
void GameObject::setHidden(bool hid) {
    _hidden = hid;
}

glm::vec3 GameObject::getPosition() {
    return _position;
}
glm::mat4 GameObject::getMatrix() {
    return _modelmatrix;
}
bool GameObject::getVoxel(glm::vec3 point) {
    if (_visible == false) return false;

	VoxelModel* voxels = _mesh->getVoxels();
	glm::vec3 halfSize = glm::vec3(0.1f) * 0.5f;
	for (size_t i = 0; i < voxels->getVoxelsCount(); i++) {
		Voxel* voxel = voxels->getVoxel(i);
		glm::vec3 position = voxel->getPosition();
		//std::cout << voxs->voxels[i].position.x << " " << voxs->voxels[i].position.y << " " << voxs->voxels[i].position.z << std::endl;
		// if (voxs->voxels[i].position == pos) {
		// 	return &voxs->voxels[i];
		// }
		glm::vec3 voxelMin = position - halfSize;
    	glm::vec3 voxelMax = position + halfSize;
		if (point.x >= voxelMin.x && point.x <= voxelMax.x &&
            point.y >= voxelMin.y && point.y <= voxelMax.y &&
            point.z >= voxelMin.z && point.z <= voxelMax.z) return true;
	}
	return false;
}

void GameObject::setMesh(Mesh* newmesh) {
	_mesh = newmesh;
}

Mesh* GameObject::getMesh() {
	return _mesh;
}

bool GameObject::raycast(glm::vec3 pos, glm::vec3 dir, float maxDist, glm::vec3& end, glm::vec3& norm, glm::vec3& iend) {
    if (_visible == false) return false;

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
	return _gm;
}