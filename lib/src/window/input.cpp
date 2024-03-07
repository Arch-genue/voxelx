#include "input.h"

#include "../utilities/logger.h"

SDL_Event Input::_sdlevent;

bool* Input::_keys;

uint32_t* Input::_frames;
uint32_t Input::_current = 0;
float Input::deltaX = 0.0f;
float Input::deltaY = 0.0f;
float Input::x = 0.0f;
float Input::y = 0.0f;
bool Input::_cursor_locked = false;
bool Input::_cursor_started = false;

std::unordered_map<uint32_t, std::function<void()> > Input::_pressedeventfunc; // Keyboard press
std::unordered_map<uint32_t, std::function<void()> > Input::_jpressedeventfunc; // Keyboard pressed
std::unordered_map<uint32_t, std::function<void()> > Input::_clickedeventfunc; // Mouse click
std::unordered_map<uint32_t, std::function<void()> > Input::_jclickedeventfunc; // Mouse clicked
std::unordered_map<uint32_t, std::function<void()> > Input::_mousemoteventfunc; // Mouse motion

#define _MOUSE_BUTTONS 1024
#define _SPECIAL_BUTTONS 1040

#define F_KEY_MAGIC 1073741824

int Input::init() {
    _keys = new bool [1512];
    _frames = new uint32_t [1512];

    memset(_keys, false, 1512 * sizeof(bool));
    memset(_frames, 0, 1512 * sizeof(uint32_t));

    _pressedeventfunc = std::unordered_map<uint32_t, std::function<void()>>();
    _jpressedeventfunc = std::unordered_map<uint32_t, std::function<void()>>();
    _clickedeventfunc = std::unordered_map<uint32_t, std::function<void()>>();
    _jclickedeventfunc = std::unordered_map<uint32_t, std::function<void()>>();

    Logger::eprint("INPUT", "Input system initialized, setup input buffers",  LOGLEVEL::INFO);
    return 0;
}

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

void Input::add_event_handler(EVENT event, uint32_t btn,  void (*func)(void)) {
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

// template<typename Func>
// void Input::add_event_handler(EVENT event, uint32_t btn, Func&& func) {
//     switch (event) {
//         case PRESSED:
//             _pressedeventfunc[btn] = func;
//             break;
//         case JPRESSED:
//             _jpressedeventfunc[btn] = func;
//             break;
//         case CLICKED:
//             _clickedeventfunc[btn] = func;
//             break;
//         case JCLICKED:
//             _jclickedeventfunc[btn] = func;
//             break;
//         case MOTION:
//             _mousemoteventfunc[btn] = func;
//             break;
//     }
// }

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

void Input::cleanup() {
    Logger::eprint("INPUT", "Clear input buffers",  LOGLEVEL::INFO);
    delete []_keys;
    delete []_frames;
}

void Input::setKey(int key, bool value) {
    if (key >= F_KEY_MAGIC) {
        uint32_t index = (_SPECIAL_BUTTONS + key - F_KEY_MAGIC);
        _keys[index] = value;
    } else {
        _keys[key] = value;
    }
}

void Input::setFrame(int key, uint32_t value) {
    if (key >= F_KEY_MAGIC) {
        uint32_t index = (_SPECIAL_BUTTONS + key - F_KEY_MAGIC);
        _frames[index] = value;
    } else {
        _frames[key] = value;
    }
}

bool Input::getKey(int keycode) {
    if (keycode >= F_KEY_MAGIC) {
        uint32_t index = (_SPECIAL_BUTTONS + keycode - F_KEY_MAGIC);
        return _keys[index];
    } else {
        return _keys[keycode];
    }
}

uint32_t Input::getFrame(int keycode) {
    if (keycode >= F_KEY_MAGIC) {
        uint32_t index = (_SPECIAL_BUTTONS + keycode - F_KEY_MAGIC);
        return _frames[index];
    } else {
        return _frames[keycode];
    }
}

bool Input::pressed(uint32_t keycode) {
    if ( keycode < 0 || (keycode >= _MOUSE_BUTTONS && keycode < _SPECIAL_BUTTONS)) return false;
    return getKey(keycode);
}
bool Input::jpressed(uint32_t keycode) {
    if ( keycode < 0 || (keycode >= _MOUSE_BUTTONS && keycode < _SPECIAL_BUTTONS)) return false;
    return getKey(keycode) && getFrame(keycode) == _current;
}

bool Input::clicked(uint32_t button) {
    int index = _MOUSE_BUTTONS + button;
    return _keys[index];
}
bool Input::jclicked(uint32_t button) {
    int index = _MOUSE_BUTTONS + button;
    return _keys[index] && _frames[index] == _current;
}

void Input::toggleCursor() {
    _cursor_locked = !_cursor_locked;
    Window::setCursorMode(_cursor_locked ? SDL_TRUE : SDL_FALSE);
}

void Input::processEvents(bool &quit) {
    while (SDL_PollEvent(&_sdlevent) != 0) {
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
    deltaX = 0.0f;
    deltaY = 0.0f;
}
