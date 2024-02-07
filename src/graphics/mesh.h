#pragma once

#include <stdlib.h>
#include "../voxels/voxel.h"

class VoxelModel;

class Mesh {
    unsigned int vao;
    unsigned int vbo;
    
    int* _meshAttributes;
    float* _meshBuffer;
    
    size_t vertices;
    VoxelModel* voxels;
    size_t vertexSize;
public:
    Mesh(VoxelModel* voxs, float* buffer, size_t vertices, int* attrs);
    Mesh(float* buffer, size_t vertices, int* attrs);
    ~Mesh();
    void create_mesh_buff();
    void create_empty_mesh_buff();

    void update();
    void reload(float* buffer, size_t vertices);
    void draw(unsigned int primitive);
    void clear();

    void setVoxel(uint num, Voxel* vox);
    Voxel* getVoxel(int num);

    VoxelModel* getVoxels();
    void setVoxels(VoxelModel* voxs);
};