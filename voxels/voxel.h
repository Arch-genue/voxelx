#pragma once

#include <string>
#include <stdint.h>
#include <glm/glm.hpp>
#include <vector>

struct Voxel {
    uint8_t id;
    glm::vec3 position;
    glm::vec4 clr;
};

class VoxModel {
public: 
    uint8_t id;
    std::string name;

    Voxel* voxels;
    int voxcount;
    glm::vec3 m_size;
   
    VoxModel(std::string filename, bool test); //Voxel* voxels, uint16_t amount
    ~VoxModel();

    void draw();
private:
    void load_vox(std::string filename);
    void gen();
    void nullvox(int voxcount);
};