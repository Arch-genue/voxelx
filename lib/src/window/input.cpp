#include "input.h"

#include "utilities/logger.hpp"
#include "imgui_impl_sdl2.h"

Input::Input(): _current(0), _deltaX(0.0f), _deltaY(0.0f), _x(0.0f), _y(0.0f), _cursor_locked(false), _cursor_started(false) {}

void Input::cursor_position_callback(SDL_MouseMotionEvent e) {
    int xpos, ypos;
    if (_window->getCursorMode()) {
        xpos = e.xrel;
        ypos = e.yrel;
    } else {
        xpos = e.x;
        ypos = e.y;
    }

    if (_cursor_started) {
        _deltaX += xpos; //xpos-x;
        _deltaY += ypos; //ypos-y;
    } else {
        _cursor_started = true; 
    }
    _x = xpos;
    _y = ypos;
}

void Input::mouse_button_callback(Uint8& button, int action) {
    if ( action == 1 ) {
        _keys[MouseButtons + button] = true;
        _frames[MouseButtons + button] = _current;
    } else if ( action == 0) {
        _keys[MouseButtons + button] = false;
        _frames[MouseButtons + button] = _current;
    }
}
void Input::key_callback(SDL_Keycode key, int action) {
    if (action == 1) {
        setKey(key, true);
        setFrame(key, _current);
    } else if (action == 0) {
        setKey(key, false);
        setFrame(key, _current);
    }
}

void Input::window_size_callback(int width, int height) {
    _window->resizeContext(width, height);
}

void Input::add_event_handler(EVENT event, uint32_t btn, std::function<void()> func) {
    switch (event) {
        case PRESSED:
            _pressedeventfunc[btn] = func;
            break;
        case JPRESSED:
            _jpressedeventfunc[btn] = func;
            break;
        case CLICKED:
            _clickedeventfunc[btn] = func;
            break;
        case JCLICKED:
            _jclickedeventfunc[btn] = func;
            break;
        case MOTION:
            _mousemoteventfunc[btn] = func;
            break;
    }
}

void Input::process_keys() {
    for (auto it = _pressedeventfunc.begin(); it != _pressedeventfunc.end(); ++it) {
        if (pressed(it->first)) it->second();
    }
    for (auto it = _jpressedeventfunc.begin(); it != _jpressedeventfunc.end(); ++it) {
        if (jpressed(it->first)) it->second();
    }
    for (auto it = _clickedeventfunc.begin(); it != _clickedeventfunc.end(); ++it) {
        if (clicked(it->first)) it->second();
    }
    for (auto it = _jclickedeventfunc.begin(); it != _jclickedeventfunc.end(); ++it) {
        if (jpressed(it->first)) it->second();
    }
}

void Input::setKey(int key, bool value) {
    if (key >= FKeyMagic) {
        uint32_t index = (SpecialButtons + key - FKeyMagic);
        _keys[index] = value;
    } else {
        _keys[key] = value;
    }
}

void Input::setFrame(int key, uint32_t value) {
    if (key >= FKeyMagic) {
        uint32_t index = (SpecialButtons + key - FKeyMagic);
        _frames[index] = value;
    } else {
        _frames[key] = value;
    }
}

bool Input::getKey(int keycode) {
    if (keycode >= FKeyMagic) {
        uint32_t index = (SpecialButtons + keycode - FKeyMagic);
        return _keys[index];
    } else {
        return _keys[keycode];
    }
}

uint32_t Input::getFrame(int keycode) {
    if (keycode >= FKeyMagic) {
        uint32_t index = (SpecialButtons + keycode - FKeyMagic);
        return _frames[index];
    } else {
        return _frames[keycode];
    }
}

bool Input::pressed(uint32_t keycode) {
    if ( keycode < 0 || (keycode >= MouseButtons && keycode < SpecialButtons)) return false;
    return getKey(keycode);
}
bool Input::jpressed(uint32_t keycode) {
    if ( keycode < 0 || (keycode >= MouseButtons && keycode < SpecialButtons)) return false;
    return getKey(keycode) && getFrame(keycode) == _current;
}

bool Input::clicked(uint32_t button) {
    int index = MouseButtons + button;
    return _keys[index];
}
bool Input::jclicked(uint32_t button) {
    int index = MouseButtons + button;
    return _keys[index] && _frames[index] == _current;
}

void Input::toggleCursor() {
    _cursor_locked = !_cursor_locked;
    _window->setCursorMode(_cursor_locked ? SDL_TRUE : SDL_FALSE);
}

void Input::processEvents(bool &quit) {
    while (SDL_PollEvent(&_sdlevent) != 0) {
        ImGui_ImplSDL2_ProcessEvent(&_sdlevent);

        Uint8 b = _sdlevent.button.button;
        int x1;int y1; SDL_GetMouseState(&x1, &y1);
        if (_sdlevent.type == SDL_QUIT) quit = true;
        if (_sdlevent.type == SDL_WINDOWEVENT) {
            if (_sdlevent.window.event == SDL_WINDOWEVENT_RESIZED ) window_size_callback(_sdlevent.window.data1, _sdlevent.window.data2);
        }

        else if (_sdlevent.type == SDL_MOUSEMOTION) cursor_position_callback(_sdlevent.motion);
        else if (_sdlevent.type == SDL_MOUSEBUTTONDOWN) mouse_button_callback(b, 1);
        else if (_sdlevent.type == SDL_MOUSEBUTTONUP) mouse_button_callback(b, 0);
        else if (_sdlevent.type == SDL_KEYDOWN && _sdlevent.key.repeat == 0) key_callback(_sdlevent.key.keysym.sym, 1);
        else if (_sdlevent.type == SDL_KEYUP) key_callback(_sdlevent.key.keysym.sym, 0);
    }
}

void Input::pullEvents() {
    _current++;
    _deltaX = 0.0f;
    _deltaY = 0.0f;
}
