#include "particlesmodel.h"


ParticlesModel::ParticlesModel(glm::vec3 size) : VoxelModel(size) {}
ParticlesModel::~ParticlesModel() {}


void ParticlesModel::addVoxel(Voxel* voxel) {
    _voxels.push_back(voxel);
}

size_t ParticlesModel::getVoxelsCount() {
    return _voxels.size();
}


void ParticlesModel::setVoxel(int num, Voxel* voxel) {
    _voxels[num] = voxel;
}
Voxel* ParticlesModel::getVoxel(int num) {
    return _voxels[num];
}
Voxel* ParticlesModel::getVoxel(glm::ivec3 position) {
    for (size_t i = 0; i < _voxels.size(); i++) {
        if (_voxels[i]->getPosition() == position) {
            return _voxels[i];
        }
    }
    return nullptr;
}


void ParticlesModel::setSize(glm::vec3 size) {
    _size = size;
}
glm::vec3 ParticlesModel::getSize() {
    return _size;
}


void ParticlesModel::setVoxelPosition(int num, glm::ivec3 position) {
    _voxels[num]->setPosition(position);
}
glm::ivec3 ParticlesModel::getVoxelPosition(int num) {
    return _voxels[num]->getPosition();
}