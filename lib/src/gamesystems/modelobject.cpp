#include "modelobject.h"

ModelObject::ModelObject(MeshModel* original_model) {
    _meshmodel = original_model;
}

void ModelObject::draw(glm::mat4 matrix, Shader* shader) {
    if (isVisible()) {
        _meshmodel->draw(GL_TRIANGLES, matrix, shader);
	}
}