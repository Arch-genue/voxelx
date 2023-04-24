#include <GL/glew.h>
#include "newwindow.h"

// #include <iostream>

SDL_Window* Window::window;
SDL_Renderer* Window::guirenderer;
SDL_GLContext Window::glContext;
SDL_Event Window::sdlEvent;

int Window::width = 0;
int Window::height = 0;

int Window::init(int width, int height, const char * title) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    
    glContext = SDL_GL_CreateContext(window);

    if (glContext == NULL) {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    glewInit();

    if (TTF_Init() != 0) {
        printf("SDL_TTF could not initialize!");
        return false;
    }

    Window::width = width;
    Window::height = height;    

    _glInit();

    return 0;
}
void Window::_glInit() {
    glViewport(0, 0, width, height);

    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
    
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void Window::_glClear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Window::swapBuffers() {
    SDL_GL_SwapWindow(window); //glfwSwapBuffers(window);
    return 1;
}

void Window::setCursorMode(SDL_bool mode) {
    SDL_SetRelativeMouseMode(mode);
}
SDL_bool Window::getCursorMode() {
    return SDL_GetRelativeMouseMode();
}

void Window::toggleFullscreen() {
    
}

void Window::exit() {
    SDL_DestroyRenderer(guirenderer);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
bool Window::isShouldClose(SDL_Event event) {
    return SDL_PollEvent(&event) != 0;
}