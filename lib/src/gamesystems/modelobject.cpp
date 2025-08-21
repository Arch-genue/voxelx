#include "modelobject.h"
#include "../utilities/logger.h"

#include "../utilities/vtype.h"

ModelObject::ModelObject(VoxelModel* original_model) {
    if (original_model == nullptr) {
        return;
    }
    _voxelModel = original_model; // ->clone();
    _sizes = _voxelModel->getSize();
    
    // float test2 = original_model->getVoxels()->get(0, 0, 1)->getColor().r;
    // float test1 = _voxelModel->getVoxels()->get(0, 0, 1)->getColor().r;

    // std::cout << test1 << " " << test2 << "\n";
    // std::cout << _voxelModel->getName() << " " << original_model->getName() << "\n";
    // std::cout << "sdfsd " << _sizes.x << " " << _sizes.y << " " << _sizes.z << "\n";
}

ModelObject::~ModelObject() {}

void ModelObject::draw(glm::mat4 matrix, Shader* shader) {
    if (isVisible()) {
        _voxelModel->getMeshPtr()->draw(GL_TRIANGLES, matrix, shader);
	}
}

VoxelModel* ModelObject::getVoxelModel() const {
    return _voxelModel; //.get();
}

void ModelObject::setVisible(bool visible) {
    _visible = visible;
}
bool ModelObject::isVisible() {
    return _visible;
}