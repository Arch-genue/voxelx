#pragma once

#include <string>
#include <stdint.h>
#include <glm/glm.hpp>
#include <vector>

struct voxel_m {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 clr;
    float size;

    float lifetime;
};

struct _voxels {
    std::vector<voxel_m> voxels;
    glm::vec3 m_size;
};

class Model {
public:    
    Model();
    ~Model();

    void draw();
private:
    void load_vox(std::string filename);
};

extern _voxels* load_model(std::string filename, const char* type);