#include "mesh.h"
#include <GL/glew.h>

Mesh::Mesh(_voxels* voxs, const float* buffer, size_t vertices, const int* attrs) : vertices(vertices) {
    voxels = voxs;

    vertexSize = 0;
    for (int i = 0; attrs[i]; i++) vertexSize += attrs[i];

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexSize * vertices, buffer, GL_STATIC_DRAW);

    //Attributes
    int offset = 0;
    for (int i = 0; attrs[i]; i++) {
        int size = attrs[i];
        glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (GLvoid*)(offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += size;
    }

    glBindVertexArray(0);
}

Mesh::Mesh(const float* buffer, size_t vertices, const int* attrs) : vertices(vertices) {
    vertexSize = 0;
    for (int i = 0; attrs[i]; i++) vertexSize += attrs[i];

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexSize * vertices, buffer, GL_STATIC_DRAW);

    //Attributes
    int offset = 0;
    for (int i = 0; attrs[i]; i++) {
        int size = attrs[i];
        glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (GLvoid*)(offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += size;
    }

    glBindVertexArray(0);
}

_voxels* Mesh::getVoxels() {
    return voxels;
}

void Mesh::setVoxels(_voxels* voxs) {
    //voxels = voxs;
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    //delete voxels;
}

void Mesh::reload(const float* buffer, size_t vertices){
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