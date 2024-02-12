#include "linesobject.h"

LinesObject::LinesObject(float* vertices, uint count) : TransformObject() {
    int attrs[2] = { 3,  0 };
    _mesh = new Mesh(vertices, count, attrs);
}

void LinesObject::updateMesh(Mesh *mesh) {
    delete _mesh;
    _mesh = mesh;
}

void LinesObject::draw() {
	if (isVisible()) {
		ResourceManager::getShader("voxel")->uniformMatrix("model", getMatrix());
		_mesh->draw(GL_LINES);

		// if (_camera != nullptr) 
		// 	_raymesh->draw(GL_LINES);
	}
};