#pragma once

#include <array>
#include <glm/glm.hpp>
#include <iostream>

// Chunk
template<int SIZE = 16>
struct Chunk {
    static constexpr int VOLUME = SIZE * SIZE * SIZE;
    static constexpr int SIZE2 = SIZE * SIZE;

    std::array<Voxel, VOLUME> voxels;
    glm::ivec3 position;

    Voxel& getVoxel(int x, int y, int z) {
        return voxels[x + y * SIZE + z * SIZE2];
    }

    const Voxel& getVoxel(int x, int y, int z) const {
        return voxels[x + y * SIZE + z * SIZE2];
    }

    bool _remesh = true; // нужно пересобирать меш
};