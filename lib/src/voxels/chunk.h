#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <array>
#include <unordered_map>

#include "graphics/shader.h"
#include "voxel.h"

#define CHUNK_SIZE 16

class VoxelStructure;

class VoxelChunk {
private:    
    static constexpr int VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
    static constexpr int SIZE2 = CHUNK_SIZE * CHUNK_SIZE;
public:
    glm::ivec3 position;
    std::array<Voxel, VOLUME> voxels;
    VoxelStructure* _structure;

    Voxel& getVoxel(int x, int y, int z);
    const Voxel& getVoxel(int x, int y, int z) const;
    Voxel& getVoxel(glm::vec3& pos);
    Voxel& getVoxel(glm::ivec3& pos);

    bool _remesh = true; // нужно пересобирать меш
};