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

enum EVENT {
    PRESSED,
    JPRESSED,
    CLICKED,
    JCLICKED,
    MOTION
};

static constexpr size_t MaxKeys = 1512;
static constexpr size_t MouseButtons = 1024;
static constexpr size_t SpecialButtons = 1040;
static constexpr int FKeyMagic = 1073741824;

/**
 * @brief Класс для считывания нажатия клавиш с клавиатуры и мыши
 * 
 */
class Input {
private:
    SDL_Event _sdlevent;
    uint32_t _current;
    
    std::array<bool, MaxKeys> _keys;
    std::array<uint32_t, MaxKeys> _frames;

    bool _cursor_locked;
    bool _cursor_started;

    float _deltaX;
    float _deltaY;
    float _x;
    float _y;

    Window* _window;

    std::unordered_map<uint32_t, std::function<void()>> _pressedeventfunc; // Keyboard press
    std::unordered_map<uint32_t, std::function<void()>> _jpressedeventfunc; // Keyboard pressed
    std::unordered_map<uint32_t, std::function<void()>> _clickedeventfunc; // Mouse click
    std::unordered_map<uint32_t, std::function<void()>> _jclickedeventfunc; // Mouse clicked
    std::unordered_map<uint32_t, std::function<void()>> _mousemoteventfunc; // Mouse motion

    void setKey(int key, bool value);
    void setFrame(int key, uint32_t value);
    bool getKey(int keycode);
    uint32_t getFrame(int keycode);

    void cursor_position_callback(SDL_MouseMotionEvent e);
    void mouse_button_callback(Uint8& button, int action);
    void key_callback(SDL_Keycode key, int action);
    void window_size_callback(int width, int height);

    Input();
    ~Input() = default;
    Input(const Input&) = delete; // запрещаем копирование
    Input& operator=(const Input&) = delete; // запрещаем присваивание
public:
    // Singleton
    static Input& instance() {
        static Input inst;
        return inst;
    }

    float get_delta_x() const { return _deltaX; };
    float get_delta_y() const { return _deltaY; };

    void setWindow(Window* window) { _window = window; }

    void add_event_handler(EVENT event, uint32_t btn, std::function<void()> func);

    void process_keys();
    void get_event();

    void cleanup();
    void processEvents(bool &quit);
    void pullEvents();

    /**
     * @brief Проверка зажата ли клавиша
     * 
     * @param keycode Код клавиши
     * @return Возвращает состояние клавиши
     */
    bool pressed(uint32_t keycode);
    /**
     * @brief Проверка была ли нажата клавиша в этом кадре
     * 
     * @param keycode Код клавиши
     * @return Возвращает состояние клавиши
     */
    bool jpressed(uint32_t keycode);
    bool clicked(uint32_t button);
    bool jclicked(uint32_t button);
    
    void toggleCursor();
    bool getCursorLock() { return _cursor_locked; }
};