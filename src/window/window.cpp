#include <GL/glew.h>
#include "window.h"
#include "../utils.h"
// #include <iostream>

SDL_Window* Window::window;
SDL_Renderer* Window::guirenderer;
SDL_GLContext Window::glContext;

bool Window::_pause;

int Window::width = 0;
int Window::height = 0;

glm::vec3 Window::sky(0);

int Window::init(int width, int height, const char* title) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::string err = SDL_GetError();
        errorprint("WINDOW", "SDL could not initialize! SDL_Error: " + RED_COLOR_STR + err + RESET_COLOR_STR,  MSGERROR);
        std::exit(1);
		return 1;
	}

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        std::string err = SDL_GetError();
        errorprint("WINDOW", "Window could not be created! SDL_Error: " + RED_COLOR_STR + err + RESET_COLOR_STR,  MSGERROR);
        std::exit(1);
        return 1;
    }
    
    glContext = SDL_GL_CreateContext(window);

    if (glContext == NULL) {
        std::string err = SDL_GetError();
        errorprint("WINDOW", "OpenGL context could not be created! SDL Error: " + RED_COLOR_STR + err + RESET_COLOR_STR,  MSGERROR);
        std::exit(1);
        return 1;
    }
    glewInit();

    if (TTF_Init() != 0) {
        errorprint("WINDOW", "SDL_TTF could not initialize!",  MSGERROR);
        std::exit(1);
        return 1;
    }

    Window::width = width;
    Window::height = height;    

    _glInit();

    return 0;
}
void Window::_glInit() {
    glViewport(0, 0, width, height);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void Window::_glClear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(sky.x, sky.y, sky.z, 1.0f);
}

bool Window::swapBuffers() {
    SDL_GL_SwapWindow(window);
    return 1;
}

void Window::setCursorMode(SDL_bool mode) {
    SDL_SetRelativeMouseMode(mode);
}
SDL_bool Window::getCursorMode() {
    return SDL_GetRelativeMouseMode();
}

void Window::setPause(bool pause) {
    _pause = pause;
}
bool Window::getPause() {
    return _pause;
}

void Window::toggleFullscreen() {
    uint32_t flags = SDL_GetWindowFlags(window);
    SDL_SetWindowFullscreen(window, (flags & SDL_WINDOW_FULLSCREEN_DESKTOP) ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void Window::exit() {
    errorprint("WINDOW", "Deleting GL context, closing window",  MSGINFO);
    SDL_DestroyRenderer(guirenderer);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
bool Window::isShouldClose(SDL_Event event) {
    return SDL_PollEvent(&event) != 0;
}