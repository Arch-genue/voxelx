#pragma once

#include "voxels/chunk.h"
#include "voxels/voxelmodel.hpp"

class VoxelApi {
public:
    static void reg (sol::state& lua) {
        lua.new_usertype<VoxelModel::ChunkType>("Chunk",
            // "getVoxel", &VoxelModel::ChunkType::getVoxel,
            // "getVoxel", static_cast<Voxel&(VoxelModel::ChunkType::*)(int, int, int)>(&VoxelModel::ChunkType::getVoxel),
            "getRemesh", [](const VoxelModel::ChunkType& v){ return v._remesh; },
            "setRemesh", [](VoxelModel::ChunkType& v, bool val){ v._remesh = val; }
        );
        lua.new_usertype<Voxel>("Voxel",
            // "setColor", [](Voxel& v, glm::vec4 clr){ v.setColor(clr); },
            "setColor", [](Voxel& v, float r, float g, float b, float a){
                v.setColor(glm::vec4(r, g, b, a));
            },
            "getColor", [](const Voxel& v){ return v.getColor(); },
            "getMat", [](const Voxel& v){ return v.mat; },
            "setMat", [](Voxel& v, uint8_t val){ v.mat = val; },
            "isVisible", [](const Voxel& v){ return v.visible; },
            "setVisible", [](Voxel& v, bool val){ v.visible = val; }
        );
        lua.new_usertype<VoxelModel>("VoxelModel",
            "getVoxel", &VoxelModel::getVoxel,
            "getHeight", &VoxelModel::getHeight,
            "getName", &VoxelModel::getName,
            "getOrCreateChunk", &VoxelModel::getOrCreateChunk
            // "getPhysicsObject", &GameObject::getPhysicsObject
        );
        lua.new_usertype<MeshModel>("MeshModel",
            // "getVoxelModel", &MeshModel::getVoxelModel,
            "getVoxelModel", &MeshModel::getVoxelModel
            // "forEachMesh", &MeshModel::forEachMesh
        );
    }
};