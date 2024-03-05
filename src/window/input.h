/**
 * @file input.h
 * @author Vlad Kartsaev
 * @brief Implementation of input system
 * @version 1.0
 * @date 2023-04-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <SDL2/SDL.h>
#include <unordered_map>
#include <functional>

#include "window.h"

typedef unsigned int uint32_t;

enum EVENT {
    PRESSED,
    JPRESSED,
    CLICKED,
    JCLICKED,
    MOTION
};

/**
 * @brief Класс для считывания нажатия клавиш с клавиатуры и мыши
 * 
 */
class Input {
private:
    static SDL_Event _sdlevent;
    static bool* _keys;
    static uint32_t* _frames;
    static uint32_t _current;

    static bool _cursor_locked;
    static bool _cursor_started;

    static std::unordered_map<uint32_t, std::function<void()>> _pressedeventfunc; // Keyboard press
    static std::unordered_map<uint32_t, std::function<void()>> _jpressedeventfunc; // Keyboard pressed
    static std::unordered_map<uint32_t, std::function<void()>> _clickedeventfunc; // Mouse click
    static std::unordered_map<uint32_t, std::function<void()>> _jclickedeventfunc; // Mouse clicked
    static std::unordered_map<uint32_t, std::function<void()>> _mousemoteventfunc; // Mouse motion

    static void setKey(int key, bool value);
    static void setFrame(int key, uint32_t value);
    static bool getKey(int keycode);
    static uint32_t getFrame(int keycode);

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

    // template<typename Func>
    // static void add_event_handler(EVENT event, uint32_t btn, Func func);

    static void add_event_handler(EVENT event, uint32_t btn, void (*func)(void));

    static void process_keys();
    static void get_event();

    static void cleanup();
    static void processEvents(bool &quit);
    static void pullEvents();

    /**
     * @brief Проверка зажата ли клавиша
     * 
     * @param keycode Код клавиши
     * @return Возвращает состояние клавиши
     */
    static bool pressed(uint32_t keycode);
    /**
     * @brief Проверка была ли нажата клавиша в этом кадре
     * 
     * @param keycode Код клавиши
     * @return Возвращает состояние клавиши
     */
    static bool jpressed(uint32_t keycode);
    static bool clicked(uint32_t button);
    static bool jclicked(uint32_t button);
    
    static void toggleCursor();
    static bool getCursorLock() { return _cursor_locked; }
};