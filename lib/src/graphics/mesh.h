/**
 * @file mesh.h
 * @author Vlad Kartsaev
 * @brief Mesh class implementation
 * @version 0.5
 * @date 2023-04-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <stdlib.h>
#include <memory>

#include "shader.h"

/**
 * @brief Класс для создания меша в с использованием OpenGL
 * 
 */
class Mesh {
    unsigned int _vao;
    unsigned int _vbo;
    
    int* _meshAttributes;
    float* _meshBuffer;
    
    size_t _vertices;
    size_t _vertexSize;
public:
    Mesh(float* buffer, size_t vertices, int* attrs);
    ~Mesh();
    std::unique_ptr<Mesh> clone() const;
    void destroy();

    void create_mesh_buff();

    void update();
    void reload(float* buffer, size_t vertices);
    void draw(unsigned int primitive);
    void draw(unsigned int primitive, glm::mat4 _modelmatrix, Shader *shader);
    void clear();
};