#include "voxelmodel.h"

VoxelModel::VoxelModel(glm::ivec3 size) {
    _minsize = glm::ivec3(0);
    this->_size = size;
}

void VoxelModel::setVoxel(int x, int y, int z, const Voxel& v) {
    glm::ivec3 cc = chunkCoord(x,y,z);
    auto& chunk = chunks[cc];
    chunk.coord = cc;
    chunk.voxels[localCoord(x,y,z)] = v;
}

Voxel* VoxelModel::getVoxel(int x, int y, int z) {
    glm::ivec3 cc = chunkCoord(x,y,z);
    auto it = chunks.find(cc);
    if (it == chunks.end()) return nullptr;
    auto lit = it->second.voxels.find(localCoord(x,y,z));
    if (lit == it->second.voxels.end()) return nullptr;
    return &lit->second;
}
Voxel* VoxelModel::getVoxel(glm::ivec3 pos) {
    return this->getVoxel(pos.x, pos.y, pos.z);
}

void VoxelModel::removeVoxel(int x, int y, int z) {
    glm::ivec3 cc = chunkCoord(x,y,z);
    auto it = chunks.find(cc);
    if (it != chunks.end()) {
        it->second.voxels.erase(localCoord(x,y,z));
    }
}

void VoxelModel::setName(std::string name) {
    _name = name;
}
std::string VoxelModel::getName() {
    return _name;
}
void VoxelModel::setVoxelVisible(glm::ivec3 position, bool visible) { 
    Voxel* voxel = getVoxel(position);
    if (voxel) {
        voxel->visible = visible;
    }
}
void VoxelModel::setMesh(std::unique_ptr<Mesh> mesh) {
    _pmesh = std::move(mesh);
}

Mesh* VoxelModel::getMeshPtr() {
    return _pmesh.get();
}

extern std::unique_ptr<VoxelModel> load_model(std::string filename, const char *type);
extern std::unique_ptr<VoxelModel> genVoxel();