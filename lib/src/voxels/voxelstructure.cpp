#include "voxelstructure.h"
#include "utilities/logger.hpp"
#include "assetmanager/assetmanager.h"

#include <chrono>
#include <fstream>

#include "utilities/utils.h"

VoxelStructure::VoxelStructure(const std::filesystem::path &path): _path(path) {
    _name = split(path.filename(), '.')[0];
    int state = this->load_structure(path);
    if (state != 0) {
        Logger::instance().log(LogLevel::ERROR, "VoxelStructure", "VoxelStructure load error brcyan<", std::string(path.filename()), ">");
        std::exit(2);
    }
}

VoxelStructure::VoxelStructure(const VoxelStructure &other) {
    other.forEachVoxel([&](const Voxel& voxel, const ChunkCoord& chunkpos, int x, int y, int z) {
        Voxel voxel1 = voxel;
        this->getVoxel(x, y, z) = voxel;
    });
}

VoxelChunk& VoxelStructure::getOrCreateChunk(int cx, int cy, int cz) {
    ChunkCoord coord{cx, cy, cz};

    VoxelChunk& chunk = chunks[coord];
    chunk._structure = this;
    chunk.position = glm::ivec3(cx, cy, cz);
    return chunk;
}

VoxelChunk &VoxelStructure::getVoxelChunk(int x, int y, int z) {
    int cx = floordiv(x, CHUNK_SIZE);
    int cy = floordiv(y, CHUNK_SIZE);
    int cz = floordiv(z, CHUNK_SIZE);

    return getOrCreateChunk(cx, cy, cz);
}

Voxel& VoxelStructure::getVoxel(int x, int y, int z) {
    int lx = floormod(x, CHUNK_SIZE);
    int ly = floormod(y, CHUNK_SIZE);
    int lz = floormod(z, CHUNK_SIZE);
    return this->getVoxelChunk(x, y, z).getVoxel(lx, ly, lz);
}

Voxel& VoxelStructure::voxel(int x, int y, int z) {
    int lx = floormod(x, CHUNK_SIZE);
    int ly = floormod(y, CHUNK_SIZE);
    int lz = floormod(z, CHUNK_SIZE);
    VoxelChunk& chunk = this->getVoxelChunk(x, y, z);
    chunk._remesh = true;
    return chunk.getVoxel(lx, ly, lz);
}

uint8_t VoxelStructure::load_structure(const std::string &filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        return -1;
    }

    std::string line;
    float clr_r = 0.0f, clr_g = 0.0f, clr_b = 0.0f, clr_a = 0.0f;

    size_t vi = 0;
    while (getline(in, line)) {
        if ( ++vi < 5 ) continue;
        auto parts = split(line, ' '); // x y z clr
        if (parts.size() < 4) continue;

        int x = std::stoi(parts[0]);
        int y = std::stoi(parts[1]);
        int z = std::stoi(parts[2]);

        //* VOXEL COLOR
        std::string hex = parts[3];
        clr_r = std::stoi(hex.substr(0, 2), nullptr, 16) / 255.0f;
        clr_g = std::stoi(hex.substr(2, 2), nullptr, 16) / 255.0f;
        clr_b = std::stoi(hex.substr(4, 2), nullptr, 16) / 255.0f;
        clr_a = hex.size() >= 8 ? std::stoi(hex.substr(6, 2), nullptr, 16) / 255.0f : clr_a = 1.0f;

        Voxel& voxel = this->getVoxel(x, y, z);
        voxel.setColor(glm::vec4(clr_r, clr_g, clr_b, clr_a));
        voxel.visible = true;
    }

    in.close();
    return 0;
}
void VoxelStructure::save_structure(VoxelStructure* voxelmodel) {
    auto start = std::chrono::high_resolution_clock::now();
    std::string filename("saved_" + voxelmodel->getName() + ".voxtxt");
    std::fstream file(filename, std::ios::in | std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        Logger::instance().log(LogLevel::SUCCESS, "RESMGR", "Cannot open file brcyan<", filename, ">");
    }

    file << "# VoxelX Model\n";
    file << "# One line per voxel\n";
    file << "# X Y Z RRGGBB\n";
    file << "\n";

    voxelmodel->forEachChunk([&](VoxelChunk &chunk, const ChunkCoord &chunkPos) {
        voxelmodel->forEachVisibleVoxelInChunk(chunk, [&](Voxel &voxel, int x, int y, int z) {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0')
            << std::setw(2) << (voxel.color.r & 0xFF)
            << std::setw(2) << (voxel.color.g & 0xFF)
            << std::setw(2) << (voxel.color.b & 0xFF)
            << std::setw(2) << (voxel.color.a & 0xFF);

            std::string hexString = oss.str();
            file << x << " " << y << " " << z << " " << hexString << "\n";
        });
    });

    file.close();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - start;

    Logger::instance().log(LogLevel::SUCCESS, "RESMGR", "Model saved brcyan<", voxelmodel->getName(), ">   brmagenta<", std::to_string(duration.count()), "s>");
}