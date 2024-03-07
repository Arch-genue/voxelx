#include "linesobject.h"

int attrs[2] = { 3,  0 };

LinesObject::LinesObject(float* vertices, uint count) : TransformObject() {
    _mesh = new Mesh(vertices, count, attrs);
}

void LinesObject::updateMesh(float* vertices, uint count) {
    delete _mesh;
    _mesh = new Mesh(vertices, count, attrs);
}

void LinesObject::draw() {
	if (isVisible()) {
		// ResourceManager::getShader("voxel")->uniformMatrix("model", getMatrix());
		_mesh->draw(GL_LINES);

		// if (_camera != nullptr) 
		// 	_raymesh->draw(GL_LINES);
	}
};