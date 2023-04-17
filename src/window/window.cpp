#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "window.h"

GLFWwindow* Window::window;
int Window::width = 0;
int Window::height = 0;

int Window::init(int width, int height, const char * title) {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    
    window = glfwCreateWindow(width, height, title, nullptr, nullptr); //glfwGetPrimaryMonitor()
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW Window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }

    glViewport(0, 0, width, height);

    Window::width = width;
    Window::height = height;

    return 0;
}
void Window::_glInit() {
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
    glfwSwapBuffers(window);
    return 1;
}

void Window::setCursorMode(int mode) {
    glfwSetInputMode(window, GLFW_CURSOR, mode);
}

void Window::toggleFullscreen() {
    //glfwSet
}

void Window::exit() {
    glfwTerminate();
}
bool Window::isShouldClose() {
    return glfwWindowShouldClose(window);
}
void Window::setShouldClose(bool flag) {
    glfwSetWindowShouldClose(window, flag);
}