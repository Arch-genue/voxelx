#pragma once

#include "voxels/chunk.h"
#include "voxels/voxelstructure.h"

class VoxelApi {
public:
    static void reg (sol::state& lua) {
        lua.new_usertype<VoxelChunk>("Chunk",
            // "getVoxel", &VoxelChunk::getVoxel,
            // "getVoxel", static_cast<Voxel&(VoxelChunk::*)(int, int, int)>(&VoxelChunk::getVoxel),
            "getRemesh", [](const VoxelChunk& v){ return v._remesh; },
            "setRemesh", [](VoxelChunk& v, bool val){ v._remesh = val; }
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
        lua.new_usertype<VoxelStructure>("VoxelModel",
            "getName", &VoxelStructure::getName,
            "getVoxel", &VoxelStructure::getVoxel,
            "getOrCreateChunk", &VoxelStructure::getOrCreateChunk
            // "getPhysicsObject", &GameObject::getPhysicsObject
        );
        lua.new_usertype<MeshModel>("MeshModel",
            // "getStructure", &MeshModel::getStructure,
            "getStructure", &MeshModel::getStructure
            // "forEachMesh", &MeshModel::forEachMesh
        );
    }
};