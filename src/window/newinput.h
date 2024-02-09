#pragma once

#include "newwindow.h"
#include <SDL2/SDL.h>

typedef unsigned int uint;

class Input {
    static SDL_Event sdlEvent;

    public:
        static bool* _keys;
        // static std::map<uint, bool> Input::_keys;
        static uint* _frames;
        static uint _current;
        static float deltaX;
        static float deltaY;
        static float x;
        static float y;
        static bool _cursor_locked;
        static bool _cursor_started;

        static int init();
        static void cleanup();
        static void processEvents(bool &quit);
        static void pullEvents();
        static void setKey(int key, bool value);
        static void setFrame(int key, uint value);
        static bool getKey(int keycode);
        static uint getFrame(int keycode);
        static bool pressed(int keycode);
        static bool jpressed(int keycode);
        static bool clicked(int button);
        static bool jclicked(int button);

        static void cursor_position_callback(SDL_MouseMotionEvent e);
        static void mouse_button_callback(Uint8& button, int action);
        static void key_callback(SDL_Keycode key, int action);
        static void window_size_callback(int width, int height);

        static void toggleCursor();
};