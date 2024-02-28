/**
 * @file newinput.h
 * @author Vlad Kartsaev
 * @brief Implementation of input system
 * @version 1.0
 * @date 2023-04-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "window.h"
#include <SDL2/SDL.h>

typedef unsigned int uint;

/**
 * @brief Класс для считывания нажатия клавиш с клавиатуры и мыши
 * 
 */
class Input {
private:
static SDL_Event sdlEvent;
    static bool* _keys;
    static uint* _frames;
    static uint _current;

    static bool _cursor_locked;
    static bool _cursor_started;

    static void setKey(int key, bool value);
    static void setFrame(int key, uint value);
    static bool getKey(int keycode);
    static uint getFrame(int keycode);

    static void cursor_position_callback(SDL_MouseMotionEvent e);
    static void mouse_button_callback(Uint8& button, int action);
    static void key_callback(SDL_Keycode key, int action);
    static void window_size_callback(int width, int height);
public:
    static float deltaX;
    static float deltaY;
    static float x;
    static float y;

    /**
     * @brief Инициализация
     * 
     * @return int Возвращает 0
     */
    static int init();
    /**
     * @brief Очистка буферов
     * 
     */
    static void cleanup();
    static void processEvents(bool &quit);
    static void pullEvents();

    /**
     * @brief Проверка зажата ли клавиша
     * 
     * @param keycode Код клавиши
     * @return Возвращает состояние клавиши
     */
    static bool pressed(int keycode);
    /**
     * @brief Проверка была ли нажата клавиша в этом кадре
     * 
     * @param keycode Код клавиши
     * @return Возвращает состояние клавиши
     */
    static bool jpressed(int keycode);
    static bool clicked(int button);
    static bool jclicked(int button);
    
    static void toggleCursor();
    static bool getCursorLock() { return _cursor_locked; }
};