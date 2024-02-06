#include "mesh.h"
#include <GL/glew.h>

Mesh::Mesh(_voxels* voxs, float* buffer, size_t vertices, int* attrs) : voxels(voxs), _meshBuffer(buffer), vertices(vertices), _meshAttributes(attrs) {
    create_mesh_buff();
}

Mesh::Mesh(float* buffer, size_t vertices, int* attrs) : _meshBuffer(buffer), vertices(vertices), _meshAttributes(attrs) {
    create_empty_mesh_buff();
}

void Mesh::create_mesh_buff() {
    vertexSize = 0;
	for (int i = 0; _meshAttributes[i]; i++) vertexSize += _meshAttributes[i];

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

    if (_meshBuffer){
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexSize * vertices, _meshBuffer, GL_STATIC_DRAW);
	} else {
		glBufferData(GL_ARRAY_BUFFER, 0, {}, GL_STATIC_DRAW);
	}

    //? Attributes
    int offset = 0;
    for (int i = 0; _meshAttributes[i]; i++) {
        int size = _meshAttributes[i];
        glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (GLvoid*)(offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += size;
    }

    glBindVertexArray(0);
}

void Mesh::create_empty_mesh_buff() {
    vertexSize = 0;
    for (int i = 0; _meshAttributes[i]; i++) vertexSize += _meshAttributes[i];

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexSize * vertices, _meshBuffer, GL_STATIC_DRAW);

    //? Attributes
    int offset = 0;
    for (int i = 0; _meshAttributes[i]; i++) {
        int size = _meshAttributes[i];
        glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (GLvoid*)(offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += size;
    }

    glBindVertexArray(0);
}


_voxels* Mesh::getVoxels() {
    return voxels;
}

void Mesh::setVoxel(uint num, voxel_m vox) {
    voxels->voxels[num] = vox;
}
voxel_m Mesh::getVoxel(int num) {
    return voxels->voxels[num];
}

void Mesh::setVoxels(_voxels* voxs) {
    // voxels = voxs;
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    //delete voxels;
}

void Mesh::update() {
    create_mesh_buff();
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}
void Mesh::reload(float* buffer, size_t vertices) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexSize * vertices, buffer, GL_STATIC_DRAW);
	this->vertices = vertices;
}

void Mesh::draw(unsigned int primitive) {
    glBindVertexArray(vao);
    glDrawArrays(primitive, 0, vertices);
    glBindVertexArray(0);
}