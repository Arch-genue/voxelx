#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
// #include <future>
// #include <vector>

#include "voxels/voxel.h"
#include "graphics/shader.h"

class VoxelStructure;
class VoxelChunk;

class VoxelMesh {
private:
    GLuint _vao; // Vertex Array Object
    GLuint _vboPos;
    GLuint _vboNormal;
    GLuint _vboColor;

    size_t _vertices;
public:
    VoxelMesh(VoxelStructure* structure, VoxelChunk& chunk);
    ~VoxelMesh();

    void draw(unsigned int primitive, glm::mat4 _modelmatrix = glm::mat4(0), Shader* shader = nullptr);

    GLuint _getvao() const { return _vao; }
    GLuint _get_pos_vbo() const { return _vboPos; }
    GLuint _get_normal_vbo() const { return _vboNormal; }
    GLuint _get_color_vbo() const { return _vboColor; }

    bool _remesh = true; // нужно пересобирать меш
};