#include "window/input.h"

class InputApi {
public:
    static void reg(sol::state& lua) {
        lua.create_named_table("Input",
            "pressed", [](SDL_KeyCode key) {
                return Input::pressed(key);
            },
            "jpressed", [](SDL_KeyCode key) {
                std::cout << "sdf\n";
                return Input::jpressed(key);
            },
            "clicked", [](SDL_KeyCode key) {
                return Input::clicked(key);
            },
            "jclicked", [](SDL_KeyCode key) {
                return Input::jclicked(key);
            }
        );
        lua.create_named_table("Key",
            "LEFT",   SDLK_LEFT,
            "RIGHT",  SDLK_RIGHT,
            "UP",     SDLK_UP,
            "DOWN",   SDLK_DOWN,
            "SPACE",  SDLK_SPACE,
            "ESCAPE", SDLK_ESCAPE
            // ... и т.д.
        );
    }
};