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
    Renderer* gui;
    Batch2D* batch;
	Camera* uicamera;
	Mesh* crosshair;

    Renderer* renderer;
public:
    GUI();
    ~GUI();

    void draw(Texture * text);
	void drawDebug(Level* level, Assets* assets, int fps, bool occlusion);

    void setRenderer(Renderer* render);
    void setFont(TTF_Font* font);

    Texture* renderText(SDL_Color clr, char* string);
    void renderButton(double x, double y);
    void renderTexture(double x, double y);
};