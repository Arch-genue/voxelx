#pragma once

#include <string>
#include <stdint.h>
#include <glm/glm.hpp>
#include <vector>

struct voxel_m {
    glm::vec3 position;
    glm::vec4 clr;
};

struct _voxels {
    voxel_m* voxels;
    size_t count;
};

class Model {
public:    
    Model();
    ~Model();

    void draw();
private:
    void load_vox(std::string filename);
    _voxels* gen_wall();
};

extern _voxels* load_model(std::string filename, const char* type, bool test);