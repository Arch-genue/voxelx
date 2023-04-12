#pragma once

#include <stdlib.h>
#include "../voxels/voxel.h"

class _voxels;

class Mesh {
    unsigned int vao;
    unsigned int vbo;
    
    size_t vertices;
    //_voxels* voxels;
public:
    Mesh(const float* buffer, size_t vertices, const int* attrs);
    ~Mesh();

    void draw(unsigned int primitive);
    void clear();

    _voxels* getVoxels();
    void setVoxels(_voxels* voxs);
};