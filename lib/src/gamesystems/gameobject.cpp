#include "gameobject.h"

#include <GL/glew.h>
#include <glm/ext.hpp>
#include "../graphics/meshmodel.h"
#include "../graphics/shader.h"
#include "../voxels/voxel.h"
#include "../loaders/resourcemanager.h"

GameObject::GameObject(GameManager* gm, std::string name, MeshModel* mesh_model, glm::vec3 position)
: TransformObject(), ModelObject(mesh_model) {
	this->_gm = gm;
	this->_id = _gm->getNewID();

	if (name == "") {
		this->_name = "GameObject #" + std::to_string(_id);
	} else {
		this->setName(name);
	}

	this->_physicsobject = new PhysicsObject(this);

	this->_campos = glm::vec3(0);
	
    this->setVisible(true);
	this->setPosition(position);

	this->_camera = nullptr;
}
GameObject::~GameObject() { delete this->_physicsobject; }

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

Camera* GameObject::getCamera() const {
	return _camera;
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
		// std::cout << this->getName() << "\n";
		// _camera->setPosition(getPosition() + _campos);
	}
};