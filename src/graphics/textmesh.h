/**
 * @file textmesh.h
 * @author Vlad Kartsaev
 * @brief Implementation of TextMesh class
 * @version 0.1
 * @date 2024-02-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include <stdlib.h>
#include <map>
#include <string>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

/**
 * @brief Структура символа
 * 
 */
struct Character {
    GLuint     TextureID; // ID текстуры глифа
    glm::ivec2 Size;      // Размеры глифа
    glm::ivec2 Bearing;   // Смещение верхней левой точки глифа
    GLuint     Advance;   // Горизонтальное смещение до начала следующего глифа
};

/**
 * @brief Класс для создания и отрисовки текста, аналог класса Mesh но для текста
 * 
 */
class TextMesh {
private:
    unsigned int _vao;
    unsigned int _vbo;
    
    int* _meshAttributes;
    float* _meshBuffer;
    
    std::map<GLchar, Character> Characters;
public:
    TextMesh(FT_Face &face);
    ~TextMesh();

    void draw(std::string text, GLfloat x, GLfloat y, GLfloat scale);
};

