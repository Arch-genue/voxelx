#include "voxelmesh.h"

#include "voxels/voxelstructure.h"

#include "graphics/renderer.h"

VoxelMesh::VoxelMesh(VoxelStructure *structure, VoxelChunk& chunk) {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vboPos);
    glGenBuffers(1, &_vboNormal);
    glGenBuffers(1, &_vboColor);
    // std::future<void> meshFuture = std::async(std::launch::async, [&]() {
    Renderer::generate_mesh(
        structure, chunk,
        this->_vertices,
        this->_vao,
        this->_vboPos,
        this->_vboNormal,
        this->_vboColor
    );
    // });
}

VoxelMesh::~VoxelMesh() {
    if (_vao) glDeleteVertexArrays(1, &_vao);
    if (_vboPos) {
        glDeleteBuffers(1, &_vboPos);
        glDeleteBuffers(1, &_vboNormal);
        glDeleteBuffers(1, &_vboColor);
    }
    _vao = 0;
    _vboPos = 0;
    _vboNormal = 0;
    _vboColor = 0;
}

void VoxelMesh::draw(unsigned int primitive, glm::mat4 _modelmatrix, Shader* shader) {
    glBindVertexArray(_vao);
    if (shader != nullptr) {
        shader->uniformMatrix("model", _modelmatrix);
    }
    glDrawArrays(primitive, 0, this->_vertices);
    glBindVertexArray(0);
}