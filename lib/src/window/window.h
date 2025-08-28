/**
 * @file window.h
 * @author Vlad Kartsaev
 * @brief Window creation
 * @version 1.0
 * @date 2023-04-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#define NO_SDL_GLEXT
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>

class SDL_Window;

/**
 * @brief Класс для создания окна
 * 
 */
class Window {
public:
    static int width;
    static int height;

    static bool _pause;

    static SDL_Window * window;
    static SDL_Renderer* guirenderer;
    static SDL_GLContext glContext;

    static glm::vec3 sky;
    
    static int init(int width, int height, const char * title);
    static int createWindow(int width, int height, const char* title);
    static int createContext();

    static void resizeContext(int w, int h); 

    static int SDL_TTF_INIT();

    static void _glInit();
    static void _glClear();
    static void exit();

    static void setCursorMode(SDL_bool mode);
    static SDL_bool getCursorMode();

    static void startFrame();
    static void renderGUI();

    static void setPause(bool pause) { _pause = pause; }
    static bool getPause() { return _pause;}

    static void toggleFullscreen();
    static bool isShouldClose(SDL_Event event);
    static void setShouldClose(bool flag);
    static bool swapBuffers(); 
};