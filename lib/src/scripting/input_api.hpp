#include "window/input.h"

class InputApi {
public:
    static void reg(sol::state& lua) {
        lua.create_named_table("Input",
            "pressed", [](SDL_KeyCode key) {
                return Input::instance().pressed(key);
            },
            "jpressed", [](SDL_KeyCode key) {
                std::cout << "sdf\n";
                return Input::instance().jpressed(key);
            },
            "clicked", [](SDL_KeyCode key) {
                return Input::instance().clicked(key);
            },
            "jclicked", [](SDL_KeyCode key) {
                return Input::instance().jclicked(key);
            }
        );
        lua.create_named_table("Key",
            "LEFT",   SDLK_LEFT,
            "RIGHT",  SDLK_RIGHT,
            "UP",     SDLK_UP,
            "DOWN",   SDLK_DOWN,
            "SPACE",  SDLK_SPACE,
            "ESCAPE", SDLK_ESCAPE,
            "A", SDLK_a,
            "B", SDLK_b,
            "C", SDLK_c,
            "D", SDLK_d,
            "E", SDLK_e,
            "F", SDLK_f,
            "G", SDLK_g,
            "H", SDLK_h,
            "I", SDLK_i,
            "J", SDLK_j,
            "K", SDLK_k,
            "L", SDLK_l,
            "M", SDLK_m,
            "N", SDLK_n,
            "O", SDLK_o,
            "P", SDLK_p,
            "Q", SDLK_q,
            "R", SDLK_r,
            "S", SDLK_s,
            "T", SDLK_t,
            "U", SDLK_u,
            "V", SDLK_v,
            "W", SDLK_w,
            "X", SDLK_x,
            "Y", SDLK_y,
            "Z", SDLK_z,
            "0", SDLK_0,
            "1", SDLK_1,
            "2", SDLK_2,
            "3", SDLK_3,
            "4", SDLK_4,
            "5", SDLK_5,
            "6", SDLK_6,
            "7", SDLK_7,
            "8", SDLK_8,
            "9", SDLK_9
        );
    }
};