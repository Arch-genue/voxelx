/**
 * @file png_loading.h
 * @author Vlad Kartsaev
 * @brief PNG Loader implementation
 * @version 0.1
 * @date 2023-04-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <string>

class Texture;
extern Texture * load_texture(std::string filename);