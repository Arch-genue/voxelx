#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
// #include <future>
// #include <vector>

#include "voxels/voxel.h"
#include "graphics/shader.h"

#include <vector>

class VoxelStructure;
class VoxelChunk;

struct VoxelInstance {
    glm::vec3 pos;
    glm::ivec3 normal;
    glm::u8vec4 color; // r,g,b,a
};

class VoxelMesh {
private:
    GLuint _vao; // Vertex Array Object
    GLuint _vboPos;
    GLuint _vboNormal;
    GLuint _vboColor;

    std::vector<VoxelInstance> _instances;
    GLuint __vao, _vboCube, _eboCube, _vboInstances;

    size_t _vertices;
public:
    VoxelMesh(size_t vertices, GLuint vao, GLuint vboPos, GLuint vboNormal, GLuint vboColor);
    VoxelMesh(VoxelChunk& chunk);
    ~VoxelMesh();

    void generateInstances(VoxelChunk& chunk);
    void drawInstances(unsigned int primitive, glm::mat4 _modelmatrix, Shader *shader);
    void draw(unsigned int primitive, glm::mat4 _modelmatrix = glm::mat4(0), Shader* shader = nullptr);

    GLuint _getvao() const { return _vao; }
    GLuint _get_pos_vbo() const { return _vboPos; }
    GLuint _get_normal_vbo() const { return _vboNormal; }
    GLuint _get_color_vbo() const { return _vboColor; }
};