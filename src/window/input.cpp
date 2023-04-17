#include "input.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>

#include <iostream>

bool* Input::_keys;
uint* Input::_frames;
uint Input::_current = 0;
float Input::deltaX = 0.0f;
float Input::deltaY = 0.0f;
float Input::x = 0.0f;
float Input::y = 0.0f;
bool Input::_cursor_locked = false;
bool Input::_cursor_started = false;

#define _MOUSE_BUTTONS 1024

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (Input::_cursor_started) {
        Input::deltaX += xpos-Input::x;
        Input::deltaY += ypos-Input::y;
    } else {
        Input::_cursor_started = true; 
    }
    Input::x = xpos;
    Input::y = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode) {
    if ( action == GLFW_PRESS ) {
        Input::_keys[_MOUSE_BUTTONS + button] = true;
        Input::_frames[_MOUSE_BUTTONS + button] = Input::_current;
    } else if ( action == GLFW_PRESS) {
        Input::_keys[_MOUSE_BUTTONS + button] = false;
        Input::_frames[_MOUSE_BUTTONS + button] = Input::_current;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (action == GLFW_PRESS) {
        Input::_keys[key] = true;
        Input::_frames[key] = Input::_current;
    } else if (action == GLFW_RELEASE) {
        Input::_keys[key] = false;
        Input::_frames[key] = Input::_current;
    }
}

void window_size_callback(GLFWwindow* window, int width, int height) {
    std::cout << width << " " << height << std::endl;
    glViewport(0, 0, width, height);
}

int Input::init() {
    GLFWwindow * window = Window::window;
    _keys = new bool [1032];
    _frames = new uint [1032];

    memset(_keys, false, 1032*sizeof(bool));
    memset(_frames, 0, 1032*sizeof(uint));
    
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    return 0;
}

bool Input::pressed(int keycode) {
    if ( keycode < 0 || keycode >= _MOUSE_BUTTONS) return false;
    return _keys[keycode];
}
bool Input::jpressed(int keycode) {
    if ( keycode < 0 || keycode >= _MOUSE_BUTTONS) return false;
    return _keys[keycode] && _frames[keycode] == _current;
}

bool Input::clicked(int button) {
    int index = _MOUSE_BUTTONS+button;
    return _keys[index];
}
bool Input::jclicked(int button) {
    int index = _MOUSE_BUTTONS+button;
    return _keys[index] && _frames[index] == _current;
}

void Input::toggleCursor() {
    _cursor_locked = !_cursor_locked;
    Window::setCursorMode(_cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void Input::pullEvents() {
    _current++;
    deltaX = 0.0f;
    deltaY = 0.0f; 
    glfwPollEvents();
}
