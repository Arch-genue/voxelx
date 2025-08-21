#include "mesh.h"
#include <GL/glew.h>
#include "../voxels/voxelmodel.h"
#include <cstring> // для memcpy, если решишь делать deep copy

Mesh::Mesh(float* buffer, size_t vertices, int* attrs)
    : _meshBuffer(buffer), _vertices(vertices), _meshAttributes(attrs), _vao(0), _vbo(0) {
    create_mesh_buff();
}
Mesh::~Mesh() {
    destroy();
}

std::unique_ptr<Mesh> Mesh::clone() const {
    float* bufferCopy = new float[_vertices * _vertexSize];
    std::memcpy(bufferCopy, _meshBuffer, _vertices * _vertexSize * sizeof(float));
    return std::make_unique<Mesh>(bufferCopy, _vertices, _meshAttributes);
}

void Mesh::destroy() {
    if (_vao) glDeleteVertexArrays(1, &_vao);
    if (_vbo) glDeleteBuffers(1, &_vbo);
    // delete[] _meshBuffer;
    _vao = 0;
    _vbo = 0;
}

void Mesh::create_mesh_buff() {
    _vertexSize = 0;
    for (int i = 0; _meshAttributes[i]; i++) {
        _vertexSize += _meshAttributes[i];
    }

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertexSize * _vertices, _meshBuffer ? _meshBuffer : nullptr, GL_STATIC_DRAW);

    int offset = 0;
    for (int i = 0; _meshAttributes[i]; i++) {
        int size = _meshAttributes[i];
        glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, _vertexSize * sizeof(float), (GLvoid*)(offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += size;
    }

    glBindVertexArray(0);
}

void Mesh::reload(float* buffer, size_t vertices) {
    _meshBuffer = buffer;
    _vertices = vertices;

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertexSize * vertices, buffer, GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void Mesh::draw(unsigned int primitive) {
    glBindVertexArray(_vao);
    glDrawArrays(primitive, 0, _vertices);
    glBindVertexArray(0);
}

void Mesh::draw(unsigned int primitive, glm::mat4 _modelmatrix, Shader* shader) {
    glBindVertexArray(_vao);
    shader->uniformMatrix("model", _modelmatrix);
    glDrawArrays(primitive, 0, _vertices);
    glBindVertexArray(0);
}
