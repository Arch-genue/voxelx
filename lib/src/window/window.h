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
#include <glm/glm.hpp>

class SDL_Window;

/**
 * @brief Класс для создания окна
 * 
 */
class Window {
public:
    Window(int width, int height, const char *title);
    ~Window();
    
    int createWindow();
    int createContext();

    void resizeContext(int w, int h); 

    void _glInit();
    void _glClear();
    void exit();

    void setCursorMode(SDL_bool mode);
    SDL_bool getCursorMode() const;

    void setSky(glm::vec3 sky) { _sky = sky; }
    const glm::vec3& getSky() const { return _sky; }

    void startFrame();
    void renderGUI();

    void setPause(bool pause) { _pause = pause; }
    bool getPause() const { return _pause;}

    int getWidth() const { return _width; }
    int getHeight() const { return _height; }

    void toggleFullscreen();
    bool pollEvent(SDL_Event& event);
    void swapBuffers(); 
private:
    const char* _title;
    
    int _width;
    int _height;

    bool _pause;

    SDL_Window* _window;
    SDL_GLContext _glContext;

    glm::vec3 _sky;
};