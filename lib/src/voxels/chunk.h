#pragma once

#include <array>
#include <glm/glm.hpp>
#include <iostream>
#include "voxelmodel.hpp"

class VoxelModel;

// Chunk
template<int SIZE = 16>
struct Chunk {
    static constexpr int VOLUME = SIZE * SIZE * SIZE;
    static constexpr int SIZE2 = SIZE * SIZE;

    std::array<Voxel, VOLUME> voxels;
    glm::ivec3 position;

    VoxelModel* voxelmodel;

    Voxel& getVoxel(int x, int y, int z) {
        if (x < 0 || y < 0 || z < 0 || x >= SIZE || y >= SIZE || z >= SIZE) {
            // пересчёт координат относительно соседнего чанка
            int globalX = position.x * SIZE + x;
            int globalY = position.y * SIZE + y;
            int globalZ = position.z * SIZE + z;

            // спрашиваем модель за вокселем
            return voxelmodel->getVoxel(globalX, globalY, globalZ);
        }
        return voxels[x + y * SIZE + z * SIZE2];
    }
    

    const Voxel& getVoxel(int x, int y, int z) const {
        return voxels[x + y * SIZE + z * SIZE2];
    }

    bool _remesh = true; // нужно пересобирать меш
};