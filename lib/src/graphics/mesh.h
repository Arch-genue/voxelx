#pragma once

#include "shader.h"
#include <GL/glew.h>

class Mesh {
public:
    int* _meshAttributes;
    size_t _vertices;
    size_t _vertexSize;

    Mesh(std::vector<float>& posBuffer, std::vector<int8_t>& normalBuffer, std::vector<uint8_t>& colorBuffer): 
        _posBuffer(posBuffer), _normalBuffer(normalBuffer), _colorBuffer(colorBuffer) {
        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vboPos);
        glGenBuffers(1, &_vboNormal);
        glGenBuffers(1, &_vboColor);
    }
    // Mesh(std::vector<float>& buffer, size_t vertices, int* attrs): _meshBuffer(buffer), _vertices(vertices), _meshAttributes(attrs) {
    //     glGenVertexArrays(1, &_vao);
    //     glGenBuffers(1, &_vbo);

    //     _vertexSize = 0;
    //     for (int i = 0; _meshAttributes[i]; i++) {
    //         _vertexSize += _meshAttributes[i];
    //     }

    //     glBindVertexArray(_vao);
    //     glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    //     glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertexSize * _vertices, this->_meshBuffer.data(), GL_STATIC_DRAW);

    //     int offset = 0;
    //     for (int i = 0; _meshAttributes[i]; i++) {
    //         glVertexAttribPointer(i, _meshAttributes[i], GL_FLOAT, GL_FALSE, _vertexSize * sizeof(float), (GLvoid*)(offset * sizeof(float)));
    //         glEnableVertexAttribArray(i);
    //         offset += _meshAttributes[i];
    //     }

    //     glBindVertexArray(0);
    // }
    ~Mesh() {
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

    void draw(unsigned int primitive, glm::mat4 _modelmatrix, Shader* shader) {
        glBindVertexArray(_vao);
        shader->uniformMatrix("model", _modelmatrix);
        glDrawArrays(primitive, 0, _vertices);
        glBindVertexArray(0);
    }
    void draw(unsigned int primitive) {
        glBindVertexArray(_vao);
        glDrawArrays(primitive, 0, _vertices);
        glBindVertexArray(0);
    }

    unsigned int _getvao() const { return _vao; }
    unsigned int _get_pos_vbo() const { return _vboPos; }
    unsigned int _get_normal_vbo() const { return _vboNormal; }
    unsigned int _get_color_vbo() const { return _vboColor; }

private:
    GLuint _vao; // Vertex Array Object
    GLuint _vboPos;
    GLuint _vboNormal;
    GLuint _vboColor;

    std::vector<float> _posBuffer;
    std::vector<int8_t> _normalBuffer;
    std::vector<uint8_t> _colorBuffer;
};