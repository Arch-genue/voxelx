#include "voxelmodel.h"

#include <iostream>

void VoxelModel::update() {
    // int remeshed = 0;
    // this->forEachChunk([&](VoxelModel::ChunkType chunk) {
    //     if (!chunk->_remesh) { return; }
    //     remeshed++;

    //     Mesh* mesh = new Mesh(this, chunk->getVoxels());

    //     chunk->setMesh(mesh);
    //     chunk->_remesh = false;
    // });
    // std::cout << "=Remeshed: " << remeshed << "\n";
}



// extern VoxelModel* load_model(std::string filename, const char *type);
// extern VoxelModel* genVoxel();