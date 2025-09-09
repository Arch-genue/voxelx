#include "chunk.h"

#include "voxelstructure.h"

Voxel& VoxelChunk::getVoxel(int x, int y, int z) {
    if (x < 0 || y < 0 || z < 0 || x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE) {
        // пересчёт координат относительно соседнего чанка
        int globalX = position.x * CHUNK_SIZE + x;
        int globalY = position.y * CHUNK_SIZE + y;
        int globalZ = position.z * CHUNK_SIZE + z;

        // спрашиваем модель за вокселем
        return _structure->getVoxel(globalX, globalY, globalZ);
    }
    return voxels[x + y * CHUNK_SIZE + z * SIZE2];
}

const Voxel& VoxelChunk::getVoxel(int x, int y, int z) const { 
    return voxels[x + y * CHUNK_SIZE + z * SIZE2];
}
Voxel &VoxelChunk::getVoxel(glm::ivec3 &pos) {
    return this->getVoxel(pos.x, pos.y, pos.z);
}
Voxel &VoxelChunk::getVoxel(glm::vec3 &pos) {
    return this->getVoxel(pos.x, pos.y, pos.z);
}