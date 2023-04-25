#pragma once

#include <string>
#include <glm/glm.hpp>
#include <vector>

struct voxel_m {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 clr;
    float size;

    float lifetime;
    bool visible;
};

struct _voxels {
    std::vector<voxel_m> voxels;
    glm::vec3 m_size;
    std::string renderSide;
};

extern _voxels* load_model(std::string filename, const char* type);
extern _voxels* genVoxel();