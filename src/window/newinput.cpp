#include "newinput.h"
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

void Input::cursor_position_callback(SDL_MouseMotionEvent e) {
    int xpos, ypos;
    if (Window::getCursorMode()) {
        xpos = e.xrel;
        ypos = e.yrel;
    } else {
        xpos = e.x;
        ypos = e.y;
    }

    if (Input::_cursor_started) {
        Input::deltaX += xpos; //xpos-Input::x;
        Input::deltaY += ypos; //ypos-Input::y;
    } else {
        Input::_cursor_started = true; 
    }
    Input::x = xpos;
    Input::y = ypos;
}

void Input::mouse_button_callback(Uint8& button, int action) {
    if ( action == 1 ) {
        Input::_keys[_MOUSE_BUTTONS + button] = true;
        Input::_frames[_MOUSE_BUTTONS + button] = Input::_current;
    } else if ( action == 0) {
        Input::_keys[_MOUSE_BUTTONS + button] = false;
        Input::_frames[_MOUSE_BUTTONS + button] = Input::_current;
    }
}

void Input::key_callback(SDL_Keycode key, int action) {
    if (action == 1) {
        Input::_keys[key] = true;
        Input::_frames[key] = Input::_current;
    } else if (action == 0) {
        Input::_keys[key] = false;
        Input::_frames[key] = Input::_current;
    }
}

void Input::window_size_callback(int width, int height) {
    glViewport(0, 0, width, height);
}

int Input::init() {
    _keys = new bool [1032];
    _frames = new uint [1032];

    memset(_keys, false, 1032*sizeof(bool));
    memset(_frames, 0, 1032*sizeof(uint));

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
    Window::setCursorMode(_cursor_locked ? SDL_TRUE : SDL_FALSE);
}

void Input::pullEvents() {
    _current++;
    deltaX = 0.0f;
    deltaY = 0.0f;
    //glfwPollEvents();
}
