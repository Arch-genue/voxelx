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

    Voxel& at(int x, int y, int z);
    const Voxel& at(int x, int y, int z) const;
    Voxel& at(glm::vec3& pos);
    Voxel& at(glm::ivec3& pos);

    template<typename Func>
    void each(Func func) {
        for (const auto& voxel : voxels) {
            func(voxel);
        }
    }
    template<typename Func>
    void eachVisible(Func func) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    Voxel& voxel = voxels[x + y * CHUNK_SIZE + z * SIZE2];
                    if (!voxel.visible) continue;

                    glm::vec3 globalPos = glm::vec3(position * CHUNK_SIZE + glm::ivec3(x, y, z));
                    func(voxel, globalPos);
                }
            }
        }
    }

    bool _dirty = true; // нужно пересобирать меш
};