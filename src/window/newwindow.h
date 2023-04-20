#pragma once

#define NO_SDL_GLEXT
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class SDL_Window;

class Window {
public:
    static int width;
    static int height;

    static SDL_Window * window;
    static SDL_GLContext glContext;
    static SDL_Event sdlEvent;
    
    static int init(int width, int height, const char * title);
    static void _glInit();
    static void _glClear();
    static void exit();

    static void setCursorMode(int mode);
    static void toggleFullscreen();
    static bool isShouldClose(SDL_Event event);
    static void setShouldClose(bool flag);
    static bool swapBuffers(); 
};