#include "modelobject.h"

ModelObject::ModelObject(VoxelModel* model) {
    if (model == nullptr) {
        return;
    }

    _voxelModel = model;
    _sizes = _voxelModel->getSize();

	_mesh = _voxelModel->getMesh();
}

ModelObject::~ModelObject() {}

void ModelObject::draw(glm::mat4 matrix, Shader* shader) {
    if (isVisible()) {
        if (_mesh != nullptr) {
		    _mesh->draw(GL_TRIANGLES, matrix, shader);
        }
	}
}

void ModelObject::setMesh(Mesh *mesh) {
    _mesh = mesh;
}

Mesh *ModelObject::getMesh() {
    return _mesh;
}

VoxelModel* ModelObject::getVoxelModel() {
    return _voxelModel;
}

void ModelObject::setVisible(bool visible) {
    _visible = visible;
}
bool ModelObject::isVisible() {
    return _visible;
}