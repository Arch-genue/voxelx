#pragma once

#include <stdlib.h>
#include <map>
#include <string>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
    GLuint     TextureID; // ID текстуры глифа
    glm::ivec2 Size;      // Размеры глифа
    glm::ivec2 Bearing;   // Смещение верхней левой точки глифа
    GLuint     Advance;   // Горизонтальное смещение до начала следующего глифа
};

class TextMesh {
private:
    unsigned int _vao;
    unsigned int _vbo;
    
    int* _meshAttributes;
    float* _meshBuffer;
    
    // size_t _vertices;
    std::map<GLchar, Character> Characters;
public:
    TextMesh(FT_Face &face);
    ~TextMesh();

    void draw(std::string text, GLfloat x, GLfloat y, GLfloat scale);
};

