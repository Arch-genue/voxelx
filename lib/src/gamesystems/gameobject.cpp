#include "gameobject.h"

#include <GL/glew.h>
#include <glm/ext.hpp>
#include "../graphics/mesh.h"
#include "../graphics/shader.h"
#include "../voxels/voxel.h"
#include "../loaders/resourcemanager.h"

GameObject::GameObject(GameManager* gm, std::string name, VoxelModel* model, glm::vec3 position) : TransformObject(), ModelObject(model) {
	_gm = gm;
	_id = _gm->getNewID();
	// gm->addGameObject(this);

	if (name == "") {
		_name = "GameObject #" + std::to_string(_id);
	} else {
		setName(name);
	}
	

	_physicsobject = _gm->getPhysicsEngine()->createRigidBody(this, getVoxelModel()->getSize());

	_campos = glm::vec3(0);
	
    setVisible(true);
	setPosition(position);
}
GameObject::~GameObject() {}

void GameObject::attachCamera(Camera* cam, glm::vec3 stdpos) {
	_camera = cam;
	if (_campos == glm::vec3(0)) {
		_campos = stdpos;
	}
	_camera->setPosition(getPosition() + _campos);
}

void GameObject::detachCamera() {
	_camera = nullptr;
}

PhysicsObject* GameObject::getPhysicsObject() {
	return _physicsobject;
}

void GameObject::setPosition(glm::vec3 position) {
	TransformObject::setPosition(position);

	_physicsobject->setPosition(position);
}

void GameObject::onTransformed() {
	ModelObject::draw(_modelmatrix, ResourceManager::getShader("voxel"));

	if (_camera != nullptr) {
		_camera->setPosition(getPosition() + _campos);
	}
	
};