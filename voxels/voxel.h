#pragma once

#include <string>
#include <stdint.h>
#include <glm/glm.hpp>
#include <vector>

struct voxel_m {
    glm::vec3 position;
    glm::vec4 clr;
    bool visible = false;
};

struct _voxels {
    voxel_m* voxels;
    size_t count;
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