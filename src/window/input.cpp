#include "input.h"

#include "../utils.h"

bool* Input::_keys;

uint* Input::_frames;
uint Input::_current = 0;
float Input::deltaX = 0.0f;
float Input::deltaY = 0.0f;
float Input::x = 0.0f;
float Input::y = 0.0f;
bool Input::_cursor_locked = false;
bool Input::_cursor_started = false;

SDL_Event Input::sdlEvent;

#define _MOUSE_BUTTONS 1024
#define _SPECIAL_BUTTONS 1040

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
        setKey(key, true);
        setFrame(key, Input::_current);
    } else if (action == 0) {
        setKey(key, false);
        setFrame(key, Input::_current);
    }
}

void Input::window_size_callback(int width, int height) {
    glViewport(0, 0, width, height);
    Window::width = width;
    Window::height = height;
}

int Input::init() {
    _keys = new bool [1512];
    _frames = new uint [1512];

    memset(_keys, false, 1512 * sizeof(bool));
    memset(_frames, 0, 1512 * sizeof(uint));

    errorprint("INPUT", "Input system initialized, setup input buffers",  MSGINFO);
    return 0;
}

void Input::cleanup() {
    errorprint("INPUT", "Clear input buffers",  MSGINFO);
    delete []_keys;
    delete []_frames;
}

void Input::setKey(int key, bool value) {
    if (key >= 1073741824) {
        uint index = (_SPECIAL_BUTTONS + key - 1073741824);
        _keys[index] = value;
    } else {
        _keys[key] = value;
    }
}

void Input::setFrame(int key, uint value) {
    if (key >= 1073741824) {
        uint index = (_SPECIAL_BUTTONS + key - 1073741824);
        _frames[index] = value;
    } else {
        _frames[key] = value;
    }
}

bool Input::getKey(int keycode) {
    if (keycode >= 1073741824) {
        uint index = (_SPECIAL_BUTTONS + keycode - 1073741824);
        return _keys[index];
    } else {
        return _keys[keycode];
    }
}

uint Input::getFrame(int keycode) {
    if (keycode >= 1073741824) {
        uint index = (_SPECIAL_BUTTONS + keycode - 1073741824);
        return _frames[index];
    } else {
        return _frames[keycode];
    }
}

bool Input::pressed(int keycode) {
    if ( keycode < 0 || (keycode >= _MOUSE_BUTTONS && keycode < _SPECIAL_BUTTONS)) return false;
    return getKey(keycode);
    // return false;
}
bool Input::jpressed(int keycode) {
    if ( keycode < 0 || (keycode >= _MOUSE_BUTTONS && keycode < _SPECIAL_BUTTONS)) return false;
    // std::cout << "sdfsd " << std::endl;
    return getKey(keycode) && getFrame(keycode) == _current;
}

bool Input::clicked(int button) {
    int index = _MOUSE_BUTTONS + button;
    return _keys[index];
}
bool Input::jclicked(int button) {
    int index = _MOUSE_BUTTONS + button;
    return _keys[index] && _frames[index] == _current;
}

void Input::toggleCursor() {
    _cursor_locked = !_cursor_locked;
    Window::setCursorMode(_cursor_locked ? SDL_TRUE : SDL_FALSE);
}

void Input::processEvents(bool &quit) {
    while (SDL_PollEvent(&sdlEvent) != 0) {
        Uint8 b = sdlEvent.button.button;
        int x1;int y1; SDL_GetMouseState(&x1, &y1);
        if (sdlEvent.type == SDL_QUIT) quit = true;
        if (sdlEvent.type == SDL_WINDOWEVENT) {
            if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED ) window_size_callback(sdlEvent.window.data1, sdlEvent.window.data2);
        }

        else if (sdlEvent.type == SDL_MOUSEMOTION) cursor_position_callback(sdlEvent.motion);
        else if (sdlEvent.type == SDL_MOUSEBUTTONDOWN) mouse_button_callback(b, 1);
        else if (sdlEvent.type == SDL_MOUSEBUTTONUP) mouse_button_callback(b, 0);
        else if (sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.repeat == 0) key_callback(sdlEvent.key.keysym.sym, 1);
        else if (sdlEvent.type == SDL_KEYUP) key_callback(sdlEvent.key.keysym.sym, 0);
    }
}

void Input::pullEvents() {
    _current++;
    deltaX = 0.0f;
    deltaY = 0.0f;
}
