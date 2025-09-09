#define GLEW_STATIC
#include <GL/glew.h>

#include "window.h"
#include "utilities/logger.hpp"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

Window::Window(int width, int height, const char* title): _title(title), _width(width), _height(height), _sky(glm::vec3(0)) {
    createWindow();

    if (createContext() == 1) {
        std::string err = SDL_GetError();
        Logger::instance().log(LogLevel::ERROR, "WINDOW", "OpenGL context could not be created! SDL Error: brred<", err, ">");
        std::exit(1);
    }

    // Инициализация Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Стили
    ImGui::StyleColorsDark();

    // Инициализация бэкендов SDL + OpenGL
    ImGui_ImplSDL2_InitForOpenGL(_window, _glContext);
    ImGui_ImplOpenGL3_Init("#version 330"); // версия OpenGL, которая у тебя

    _glInit();
}

Window::~Window() {
    Logger::instance().log(LogLevel::INFO, "WINDOW", "Deleting GL context, closing window");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    
    // SDL_DestroyRenderer(guirenderer);
    SDL_GL_DeleteContext(_glContext);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

int Window::createWindow() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::string err = SDL_GetError();
        Logger::instance().log(LogLevel::ERROR, "WINDOW", "SDL could not initialize! SDL_Error: brred<", err, ">");
        std::exit(1);
	}

    _window = SDL_CreateWindow(_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (_window == NULL) {
        std::string err = SDL_GetError();
        Logger::instance().log(LogLevel::ERROR, "WINDOW", "Window could not be created! SDL_Error: brred<", err, ">");
        std::exit(1);
    }

    return 0;
}

int Window::createContext() {
    _glContext = SDL_GL_CreateContext(_window);
    if (_glContext == NULL) { 
        std::string err = SDL_GetError();
        Logger::instance().log(LogLevel::ERROR, "Window", "OpenGL context could not be created! SDL Error: brred<", err, ">");
        std::exit(1);
    }

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        Logger::instance().log(LogLevel::ERROR, "Window", "GLEW error: brred<", (const char*)glewGetErrorString(err), ">");
        std::exit(1);
    }

    return 0;
}

void Window::resizeContext(int w, int h) {
    _width = w; _height = h;
    glViewport(0, 0, _width, _height);
}

void Window::_glInit() {
    glViewport(0, 0, _width, _height);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void Window::_glClear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(_sky.x, _sky.y, _sky.z, 1.0f);
}

void Window::swapBuffers() {
    SDL_GL_SwapWindow(_window);
}

void Window::setCursorMode(SDL_bool mode) { SDL_SetRelativeMouseMode(mode); }
SDL_bool Window::getCursorMode() const { return SDL_GetRelativeMouseMode(); }

void Window::startFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void Window::renderGUI() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::toggleFullscreen() {
    uint32_t flags = SDL_GetWindowFlags(_window);
    SDL_SetWindowFullscreen(_window, (flags & SDL_WINDOW_FULLSCREEN_DESKTOP) ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
}

bool Window::pollEvent(SDL_Event& event) {
    return SDL_PollEvent(&event) != 0;
}