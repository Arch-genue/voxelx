#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>

#include "voxel.h"
#include "../graphics/mesh.h"

#define CHUNK_SIZE 32

class Mesh;

// хэш для glm::ivec3
struct ivec3_hash {
    std::size_t operator()(const glm::ivec3& v) const noexcept {
        std::size_t hx = std::hash<int>()(v.x);
        std::size_t hy = std::hash<int>()(v.y);
        std::size_t hz = std::hash<int>()(v.z);

        // простое комбинирование хэшей
        std::size_t seed = hx;
        seed ^= hy + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hz + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};
#include <iostream>
struct Chunk {
    glm::ivec3 coord;
    std::unordered_map<glm::ivec3, Voxel, ivec3_hash> voxels;
};

class VoxelModel {
private:
    std::unordered_map<glm::ivec3, Chunk, ivec3_hash> chunks;
    std::string _name;
    glm::ivec3 _size;
    glm::ivec3 _minsize;
    
    //* Предзагруженный стандартный меш
    std::unique_ptr<Mesh> _pmesh;

    glm::ivec3 chunkCoord(int x, int y, int z) const {
        return {x/CHUNK_SIZE, y/CHUNK_SIZE, z/CHUNK_SIZE};
    }

    glm::ivec3 localCoord(int x, int y, int z) const {
        return {x % CHUNK_SIZE, y % CHUNK_SIZE, z % CHUNK_SIZE};
    }

public:
    VoxelModel(): _minsize(0) {}
    VoxelModel(glm::ivec3 size);
    ~VoxelModel() {
        std::cout << "Model destroyed \n";
    }

    void setVoxel(int x, int y, int z, const Voxel& v);
    Voxel* getVoxel(int x, int y, int z);
    Voxel* getVoxel(glm::ivec3 pos);

    void removeVoxel(int x, int y, int z);

    void setName(std::string name);
    std::string getName();

    void setVoxelVisible(glm::ivec3 position, bool visible);

    std::unordered_map<glm::ivec3, Chunk, ivec3_hash> get_chunks() {
        return chunks;
    }

    void setMesh(std::unique_ptr<Mesh> mesh);
    Mesh* getMeshPtr();

    void setMinSize(glm::ivec3 size) { _minsize = size; }
    void setSize(glm::ivec3 size) { _size = size; }
    glm::ivec3 getMinSize() const { return _minsize; }
    glm::ivec3 getSize() const { return _size; }

    template<typename Func>
    void forEachVoxel(Func func) {
        for (auto& [chunkPos, chunk] : chunks) {
            for (auto& [localPos, voxel] : chunk.voxels) {
                func(&voxel);
            }
        }
    }
};