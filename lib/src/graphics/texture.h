/**
 * @file texture.h
 * @author Vlad Kartsaev
 * @brief Implementation of OpenGL Texture 
 * @version 0.1
 * @date 2023-04-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include <string>

class Texture {
public:
    unsigned int id;
    int width;
    int height;

    Texture(unsigned int id, int width, int height);
    Texture(unsigned char* data, int width, int height);
    ~Texture();

    void bind();
};

extern Texture * load_texture(std::string filename);