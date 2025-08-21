#pragma once

#include <glm/glm.hpp>

struct Voxel {
    glm::ivec3 position = glm::ivec3(0, 0, 0);
    glm::vec4 color = glm::vec4(1, 0, 0, 1);
    bool visible = true;
};