/**
 * @file cube.h
 * @author Vlad Kartsaev
 * @brief Create a cube with a given size
 * @version 0.1
 * @date 2024-02-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <glm/glm.hpp>
#include "mesh.h"

class Cube : public Mesh {
private:
    /* data */
public:
    Cube();
    ~Cube();

    Mesh* getMesh();

    // getVoxelModel();

};


