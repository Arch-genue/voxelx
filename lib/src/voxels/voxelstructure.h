#pragma once

#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <filesystem>
#include <unordered_map>

#include "voxel.h"
#include "chunk.h"
#include "graphics/shader.h"

struct ChunkCoord {
    int x, y, z;

    bool operator==(const ChunkCoord& other) const noexcept {
        return x == other.x && y == other.y && z == other.z;
    }
};

struct ChunkCoordHash {
    std::size_t operator()(const ChunkCoord& c) const noexcept {
        std::size_t h1 = std::hash<int>{}(c.x);
        std::size_t h2 = std::hash<int>{}(c.y);
        std::size_t h3 = std::hash<int>{}(c.z);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

class VoxelStructure {
public:
    using ChunkMap = std::unordered_map<ChunkCoord, VoxelChunk, ChunkCoordHash>;

    VoxelStructure(const std::filesystem::path& path);
    VoxelStructure(const VoxelStructure& other); // Конструктор копирования
    ~VoxelStructure() = default;

    void setName(std::string name) { _name = name; }
    std::string getName() const { return _name; }

    // Получить или создать чанк
    VoxelChunk& getOrCreateChunk(int cx, int cy, int cz);
    VoxelChunk& getVoxelChunk(int x, int y, int z);

    Voxel& getVoxel(int x, int y, int z);
    Voxel& voxel(int x, int y, int z);

    template<typename Func>
    void forEachVoxel(Func func) {
        for (auto& [chunkPos, chunk] : chunks) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                for (int y = 0; y < CHUNK_SIZE; ++y) {
                    for (int x = 0; x < CHUNK_SIZE; ++x) {
                        func(chunk.getVoxel(x, y, z), chunkPos, x, y, z);
                    }
                }
            }
        }
    }

    template<typename Func>
    void forEachVoxel(Func func) const {
        for (auto& [chunkPos, chunk] : chunks) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                for (int y = 0; y < CHUNK_SIZE; ++y) {
                    for (int x = 0; x < CHUNK_SIZE; ++x) {
                        func(chunk.getVoxel(x, y, z), chunkPos, x, y, z);
                    }
                }
            }
        }
    }

    template<typename Func>
    void forEachVisibleVoxelInChunk(VoxelChunk& chunk, Func func) {
        glm::ivec3 chunkPos = chunk.position;
        for (int z = 0; z < CHUNK_SIZE; ++z)
            for (int y = 0; y < CHUNK_SIZE; ++y)
                for (int x = 0; x < CHUNK_SIZE; ++x) {
                    Voxel vox = chunk.voxels[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)];
                    if (vox.visible) {
                        int gx = chunkPos.x * CHUNK_SIZE + x;
                        int gy = chunkPos.y * CHUNK_SIZE + y;
                        int gz = chunkPos.z * CHUNK_SIZE + z;
                        func(vox, gx, gy, gz);
                    }
                }
    }

    template<typename Func>
    void forEachChunk(Func func) {
        for (auto& [chunkPos, chunk] : chunks) {
            func(chunk, chunkPos);
        }
    }

    uint8_t load_structure(const std::string &filename);
    void save_structure(VoxelStructure *voxelmodel);
private:
    ChunkMap chunks;

    std::string _name;
    std::string _path;
};