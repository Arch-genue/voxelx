#include "mesh.h"
#include <GL/glew.h>
#include "../voxels/voxelmodel.h"
// class Voxel;

Mesh::Mesh(VoxelModel* voxs, float* buffer, size_t vertices, int* attrs) : _voxels(voxs), _meshBuffer(buffer), _vertices(vertices), _meshAttributes(attrs) {
    create_mesh_buff();
}
Mesh::Mesh(float* buffer, size_t vertices, int* attrs) : _meshBuffer(buffer), _vertices(vertices), _meshAttributes(attrs) {
    create_empty_mesh_buff();
}
Mesh::~Mesh() {
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    //delete voxels;
}

Mesh* Mesh::clone() {
    Mesh* mesh = new Mesh(_voxels, _meshBuffer, _vertices, _meshAttributes);
    mesh->_vao = _vao;
    mesh->_vbo = _vbo;

    mesh->_meshAttributes = _meshAttributes;
    mesh->_meshBuffer = _meshBuffer;
    mesh->_vertices = _vertices;
    mesh->_voxels = _voxels;
    mesh->_vertexSize = _vertexSize;

    return mesh;
}

void Mesh::create_mesh_buff() {
    _vertexSize = 0;
	for (int i = 0; _meshAttributes[i]; i++) _vertexSize += _meshAttributes[i];

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    if (_meshBuffer){
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertexSize * _vertices, _meshBuffer, GL_STATIC_DRAW);
	} else {
		glBufferData(GL_ARRAY_BUFFER, 0, {}, GL_STATIC_DRAW);
	}

    //? Attributes
    int offset = 0;
    for (int i = 0; _meshAttributes[i]; i++) {
        int size = _meshAttributes[i];
        glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, _vertexSize * sizeof(float), (GLvoid*)(offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += size;
    }

    glBindVertexArray(0);
}

void Mesh::create_empty_mesh_buff() {
    _vertexSize = 0;
    for (int i = 0; _meshAttributes[i]; i++) _vertexSize += _meshAttributes[i];

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertexSize * _vertices, _meshBuffer, GL_STATIC_DRAW);

    //? Attributes
    int offset = 0;
    for (int i = 0; _meshAttributes[i]; i++) {
        int size = _meshAttributes[i];
        glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, _vertexSize * sizeof(float), (GLvoid*)(offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += size;
    }

    glBindVertexArray(0);
}

VoxelModel* Mesh::getVoxels() {
    return _voxels;
}

void Mesh::setVoxel(uint num, Voxel* vox) {
    // voxels->setVoxel(num, vox);
}
Voxel* Mesh::getVoxel(int num) {
    return _voxels->getVoxel(glm::ivec3(num));
}

void Mesh::setVoxels(VoxelModel* voxels) {
    // voxels = voxs;
}

void Mesh::update() {
    create_mesh_buff();
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
}
void Mesh::reload(float* buffer, size_t vertices) {
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertexSize * vertices, buffer, GL_STATIC_DRAW);
	this->_vertices = vertices;
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