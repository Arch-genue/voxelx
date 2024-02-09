/**
 * @file input.h
 * @author Vlad Kartsaev
 * @brief Input system implementation
 * @version 0.9
 * @date 2023-04-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "window.h"

typedef unsigned int uint;

/**
 * @brief Класс системы ввода
 * 
 */
class Input {
    public:
        static bool* _keys;
        static uint* _frames;
        static uint _current;
        static float deltaX;
        static float deltaY;
        static float x;
        static float y;
        static bool _cursor_locked;
        static bool _cursor_started;

        static int init();
        static void pullEvents();
        static bool pressed(int keycode);
        static bool jpressed(int keycode);
        static bool clicked(int button);
        static bool jclicked(int button);

        static void toggleCursor();
};