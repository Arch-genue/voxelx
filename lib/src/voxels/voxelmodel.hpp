#pragma once

#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>

#include "voxel.h"
#include "chunk.h"
#include <unordered_map>

#define CHUNK_SIZE 16

inline int floordiv(int a, int b) {
    int q = a / b;
    int r = a % b;
    if ((r != 0) && ((r > 0) != (b > 0))) --q; // сместить к floor
    return q;
}

inline int floormod(int a, int b) {
    int m = a % b;
    if (m < 0) m += (b > 0 ? b : -b);
    return m;
}

//---------------------------
struct ChunkCoord {
    int x, y, z;

    bool operator==(const ChunkCoord& other) const noexcept {
        return x == other.x && y == other.y && z == other.z;
    }
};

struct ChunkCoordHash {
    std::size_t operator()(const ChunkCoord& c) const noexcept {
        // простейший хэш
        std::size_t h1 = std::hash<int>{}(c.x);
        std::size_t h2 = std::hash<int>{}(c.y);
        std::size_t h3 = std::hash<int>{}(c.z);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

class VoxelModel {
public:
    using ChunkType = Chunk<CHUNK_SIZE>;
    using ChunkTypePtr = ChunkType*;
    using ChunkMap = std::unordered_map<ChunkCoord, ChunkType, ChunkCoordHash>;

    void setName(std::string name) { _name = name; }
    std::string getName() { return _name; }
    
    VoxelModel() = default;
    ~VoxelModel() = default;

    // Получить или создать чанк
    ChunkType& getOrCreateChunk(int cx, int cy, int cz) {
        ChunkCoord coord{cx, cy, cz};

        ChunkType& chunk = chunks[coord];
        chunk.voxelmodel = this;
        chunk.position = glm::ivec3(cx, cy, cz);
        return chunk;
    }

    // Получить воксель по мировым координатам
    Voxel& voxel(int x, int y, int z) {
        int cx = floordiv(x, CHUNK_SIZE);
        int cy = floordiv(y, CHUNK_SIZE);
        int cz = floordiv(z, CHUNK_SIZE);
        
        int lx = floormod(x, CHUNK_SIZE);
        int ly = floormod(y, CHUNK_SIZE);
        int lz = floormod(z, CHUNK_SIZE);

        ChunkType& chunk = getOrCreateChunk(cx, cy, cz);
        chunk._remesh = true;
        return chunk.getVoxel(lx, ly, lz);
    }

    // Получить воксель по мировым координатам
    Voxel& getVoxel(int x, int y, int z) {
        int cx = floordiv(x, CHUNK_SIZE);
        int cy = floordiv(y, CHUNK_SIZE);
        int cz = floordiv(z, CHUNK_SIZE);
        
        int lx = floormod(x, CHUNK_SIZE);
        int ly = floormod(y, CHUNK_SIZE);
        int lz = floormod(z, CHUNK_SIZE);

        return getOrCreateChunk(cx, cy, cz).getVoxel(lx, ly, lz);
    }

    ChunkType& getVoxelChunk(int x, int y, int z) {
        int cx = floordiv(x, CHUNK_SIZE);
        int cy = floordiv(y, CHUNK_SIZE);
        int cz = floordiv(z, CHUNK_SIZE);
        
        int lx = floormod(x, CHUNK_SIZE);
        int ly = floormod(y, CHUNK_SIZE);
        int lz = floormod(z, CHUNK_SIZE);

        return getOrCreateChunk(cx, cy, cz);
    }

    // Перебор всех вокселей
    template<typename Func>
    void forEachVoxel(Func func) {
        for (auto& [chunkPos, chunk] : chunks) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                for (int y = 0; y < CHUNK_SIZE; ++y) {
                    for (int x = 0; x < CHUNK_SIZE; ++x) {
                        func(chunk.getVoxel(x, y, z), chunkPos, x, y, z);
                        // chunkPos — координаты чанка, x/y/z — локальные координаты внутри чанка
                    }
                }
            }
        }
    }

    // Возвращает высоту самого верхнего видимого вокселя в мировых координатах (x,z)
    int getHeight(int worldX, int worldZ) const {
        int maxY = std::numeric_limits<int>::min();

        // Вычисляем координаты чанка по X и Z
        int cx = floordiv(worldX, CHUNK_SIZE);
        int cz = floordiv(worldZ, CHUNK_SIZE);

        // Перебираем все чанки по Y
        for (int cy = -100; cy <= 100; ++cy) {  // пример диапазона по Y
            ChunkCoord coord{cx, cy, cz};

            auto it = chunks.find(coord);
            if (it == chunks.end()) continue;

            const ChunkType& chunk = it->second;

            // локальные координаты внутри чанка
            int lx = floormod(worldX, CHUNK_SIZE);
            int lz = floormod(worldZ, CHUNK_SIZE);

            // Перебор по Y сверху вниз
            for (int ly = CHUNK_SIZE - 1; ly >= 0; --ly) {
                const Voxel& vox = chunk.getVoxel(lx, ly, lz);
                if (vox.visible) {
                    int height = cy * CHUNK_SIZE + ly;
                    if (height > maxY) maxY = height;
                    break;  // нашли верхний, выходим
                }
            }
        }

        return (maxY == std::numeric_limits<int>::min()) ? 0 : maxY;
    }

    template<typename Func>
    void forEachVisibleVoxelInChunk(ChunkType& chunk, Func func) {
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

    // Перебор всех чанков
    template<typename Func>
    void forEachChunk(Func func) {
        for (auto& [chunkPos, chunk] : chunks) {
            func(chunk, chunkPos);
        }
    }

private:
    ChunkMap chunks;
    std::string _name;
};