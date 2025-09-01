#include "gameobject.h"

#include <GL/glew.h>
#include <glm/ext.hpp>
#include "../graphics/meshmodel.h"
#include "../graphics/shader.h"
#include "../voxels/voxel.h"
#include "../loaders/resourcemanager.h"

GameObject::GameObject(const std::string& name, MeshModel* mesh_model, const glm::vec3& position)
: TransformObject(), ModelObject(mesh_model) {
	this->_id = GameManager::instance().getNewID();

	if (name == "") {
		this->_name = "GameObject #" + std::to_string(_id);
	} else {
		this->setName(name);
	}

	this->_physicsobject = std::make_unique<PhysicsObject>(this);

	this->_campos = glm::vec3(0);
	
    this->setVisible(true);
	this->setPosition(position);

	this->_camera = nullptr;
}

void GameObject::attachCamera(Camera* cam, const glm::vec3& stdpos) {
	_camera = cam;
	if (_campos == glm::vec3(0)) {
		_campos = stdpos;
	}
	_camera->setPosition(getPosition() + _campos);
}

void GameObject::setPosition(const glm::vec3& position) {
	TransformObject::setPosition(position);

	_physicsobject->setPosition(position);
}

void GameObject::onTransformed() {
	ModelObject::draw(_modelmatrix, ResourceManager::getShader("voxel"));

	if (_camera != nullptr) {
		_camera->setPosition(getPosition() + _campos);
	}
}
void GameObject::attachScript(const std::string &name) {
	this->_scripts.push_back(name);
	ScriptSystem::instance().startScript(name, this);
};