#pragma once

#include <string>
#include <stdint.h>
#include <glm/glm.hpp>
#include <vector>

struct Voxel {
    uint8_t id;
    glm::vec3 position;
};

class VoxModel {
public: 
    uint8_t id;
    std::string name;

    Voxel* voxels;
    uint16_t voxcount;
   
    VoxModel(std::string filename); //Voxel* voxels, uint16_t amount
    ~VoxModel();

    void draw();
private:
    void load_vox(std::string filename);
    void nullvox();
};