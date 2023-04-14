#include "gameobject.h"

#include <GL/glew.h>
#include <glm/ext.hpp>
#include "../graphics/mesh.h"
#include "../graphics/shader.h"
#include "../voxels/voxel.h"

#include <iostream>

GameObject::GameObject(Renderer* rndr, _voxels voxels, Shader *sh) {
    //
    render = rndr;
	_voxels* voxs = new _voxels;
	voxs->voxels = voxels.voxels;
	voxs->m_size = voxels.m_size;
	
    //voxels = voxs;
    shader = sh;

    //
    modelmatrix = glm::mat4(1.0f);;
    position = glm::vec3(0);
    rotAngle = 0.0f;
    rotAxis = glm::vec3(1);
    scaling = glm::vec3(0.1f);

    //
    mass = 100; //100Kg
    collider = true; //Solid

    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

    //!CREATE MESH!
	mesh = render->render(voxs);
	//delete voxs;
}
GameObject::~GameObject() {}

void GameObject::draw() {
    modelmatrix = glm::scale(modelmatrix, scaling);
    modelmatrix = glm::translate(modelmatrix, position);
    
    //glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0f), rotAngle, rotAxis);
    //modelmatrix = modelmatrix * rotateMatrix;

    shader->uniformMatrix("model", modelmatrix);
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

	mesh = render->render(voxels);
	lastposition = position;

    // if (impulseTime > 0.0f) {
    //     acceleration = impulse * impulseTime;
    //     velocity += acceleration * deltaTime;
    //     impulseTime -= deltaTime;
    // } else {
    //     velocity = glm::vec3(0.0f);
    //     impulse = glm::vec3(0.0f);
    //     impulseTime = 0.0f;
    // }
    
    // if (!true) velocity += acceleration * deltaTime;
    
    //position += velocity * 10.0f * deltaTime;
}

void GameObject::translate(float val, glm::vec3 vec) {
	position += vec*val;
	_voxels* voxels = mesh->getVoxels();

	for (size_t i = 0; i < voxels->voxels.size(); i++) {
		voxels->voxels.at(i).position += position;
	}
	//delete mesh;

	mesh = render->render(voxels);
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

glm::vec3 GameObject::getPosition() {
    return position;
}

glm::mat4 GameObject::getMatrix() {
    return modelmatrix;
}

bool GameObject::getVoxel(glm::vec3 point) {
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