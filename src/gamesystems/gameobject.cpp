#include "gameobject.h"

#include <GL/glew.h>
#include <glm/ext.hpp>
#include "../graphics/mesh.h"
#include "../graphics/shader.h"
#include "../voxels/voxel.h"

#include <iostream>

GameObject::GameObject(Renderer* rndr, const char* model) {
    //
    renderer = rndr;
	
    //
    modelmatrix = glm::mat4(1.0f);;
    position = glm::vec3(0);
    rotAngle = 0.0f;
    rotAxis = glm::vec3(1);
    scaling = glm::vec3(0.1f);

    setVisible(true);

    //
    mass = 100; //100Kg
    collider = true; //Solid

    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

    //!CREATE MESH!
	mesh = renderer->render(renderer->getRowModel(model));
	//delete voxs;
}
GameObject::~GameObject() {}

void GameObject::draw() {
    if (visible == false) return;
    modelmatrix = glm::scale(modelmatrix, scaling);
    modelmatrix = glm::translate(modelmatrix, position);
    
    //glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0f), rotAngle, rotAxis);
    //modelmatrix = modelmatrix * rotateMatrix;

    renderer->getDefaultShader()->uniformMatrix("model", modelmatrix);
    mesh->draw(GL_TRIANGLES);

    modelmatrix = glm::mat4(1.0f);
}

void GameObject::setImpulse(glm::vec3 force) {
    impulse += (force*100.0f) / mass;
    impulseTime = 1.0f;
}

void GameObject::applyForce(glm::vec3 force) {
	acceleration += force / mass;
}

void GameObject::updatePhysics(float deltaTime) {
	velocity += acceleration * 500.0f * deltaTime;
	position += velocity;
	acceleration = glm::vec3(0.0f);
	if (position == lastposition) return;
	_voxels* voxels = mesh->getVoxels();

	for (size_t i = 0; i < voxels->voxels.size(); i++) {
		voxels->voxels.at(i).position += velocity*deltaTime;
	}
	//delete mesh;

	mesh = renderer->render(voxels);
	lastposition = position;
}

void GameObject::translate(float val, glm::vec3 vec) {
	position += vec*val;
	_voxels* voxels = mesh->getVoxels();

	for (size_t i = 0; i < voxels->voxels.size(); i++) {
		voxels->voxels.at(i).position += position;
	}
	//delete mesh;

	mesh = renderer->render(voxels);
}

void GameObject::rotate(float angle, glm::vec3 rot) {
    rotAngle += angle;
    rotAxis = rot; 
}

void GameObject::setPosition(glm::vec3 pos) {
	position = pos;
	// _voxels* voxels = mesh->getVoxels();

	// for (size_t i = 0; i < voxels->voxels.size(); i++) {
	// 	voxels->voxels.at(i).position += position;
	// }
	//delete mesh;

	//mesh = render->render(voxels);
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