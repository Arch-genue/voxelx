/**
 * @file gui.h
 * @author Vlad Kartsaev
 * @brief GUI implementation
 * @version 0.1
 * @date 2023-04-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "texture.h"
#include "renderer.h"

#include <iostream>

class Batch2D;
class Camera;
class Level;
class Assets;
class Mesh;

class GUI {
    TTF_Font* defaultFont;
    Batch2D* batch;
	Camera* uicamera;
	Mesh* crosshair;

    Texture* blocks;
public:
    GUI();
    ~GUI();

    void draw();
	void drawDebug(Level* level, Assets* assets, int fps, bool occlusion);

    void setFont(TTF_Font* font);

    Texture* renderText(SDL_Color clr, const char* string);
    void renderButton(double x, double y);
    void renderTexture(double x, double y);
};