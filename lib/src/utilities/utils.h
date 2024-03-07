/**
 * @file utils.h
 * @author Vlad Kartsaev
 * @brief Some helper functions
 * @version 1.0
 * @date 2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <cstring>
#include <string>
#include <regex>
#include <chrono>
#include <glm/glm.hpp>
#include <algorithm>

// Специальные символы ANSI для цвета текста
#define RESET_COLOR         "\033[0m"
#define BLACK_COLOR         "\033[0;30m"
#define RED_COLOR           "\033[0;31m"
#define GREEN_COLOR         "\033[0;32m"
#define YELLOW_COLOR        "\033[0;33m"
#define BLUE_COLOR          "\033[0;34m"
#define MAGENTA_COLOR       "\033[0;35m"
#define CYAN_COLOR          "\033[0;36m"
#define WHITE_COLOR         "\033[0;37m"

// Специальные символы ANSI для яркого цвета текста
#define BRIGHT_BLACK_COLOR  "\033[1;30m"
#define BRIGHT_RED_COLOR    "\033[1;31m"
#define BRIGHT_GREEN_COLOR  "\033[1;32m"
#define BRIGHT_YELLOW_COLOR "\033[1;33m"
#define BRIGHT_BLUE_COLOR   "\033[1;34m"
#define BRIGHT_MAGENTA_COLOR "\033[1;35m"
#define BRIGHT_CYAN_COLOR   "\033[1;36m"
#define BRIGHT_WHITE_COLOR  "\033[1;37m"

#define BLACK_BACKGROUND        "\033[40m"
#define WHITE_BACKGROUND        "\033[47m"
#define BRIGHT_YELLOW_BACKGROUND "\033[103m"


const std::string RESET_COLOR_STR = RESET_COLOR;
const std::string RED_COLOR_STR = RED_COLOR;
const std::string BLUE_COLOR_STR = BLUE_COLOR;
const std::string GREEN_COLOR_STR = GREEN_COLOR;
const std::string YELLOW_COLOR_STR = YELLOW_COLOR;
const std::string WHITE_COLOR_STR = WHITE_COLOR;
const std::string MAGENTA_COLOR_STR = MAGENTA_COLOR;

#include <iostream>

/**
 * @brief Разделяет строку на строки по разделителю
 * 
 * @param bufstr Буферный массив строк
 * @param str Исходная строка
 * @param separator Разделитель
 */
extern void split(std::string* bufstr, std::string str, char separator);

/**
 * @brief Разделяет строку с переменной VoxParticles 
 * 
 * @param bufstr Буферный массив строк
 * @param str Исходная строка
 */
extern void splitvalue(std::string* bufstr, std::string str);

/**
 * @brief Проверка является ли эта строка числом
 * 
 * @param str Строка
 * @return true 
 * @return false 
 */
extern bool isNumeric(const std::string& str);

/**
 * @brief Парсинг параметра который может содержать функцию random()
 * 
 * @param color Значение параметра
 * @return float* Массив float содержащий значения x, y либо одно значение
 */
extern float* random_keyword_parse(std::string color);

/**
 * @brief Вызывает функцию и возвращает время затраченное на ее выполнение
 * 
 * @param func Функция
 * @param args Аргументы функции
 * @return float Время 
 */
template<typename Func, typename... Args>
float measureFunctionTime(Func &func, Args&&... args) {
    auto start = std::chrono::steady_clock::now(); // Засекаем начало времени

    // Вызываем переданную функцию с переданными аргументами
    func(std::forward<Args>(args)...);

    auto end = std::chrono::steady_clock::now(); // Засекаем конец времени

    // Вычисляем время выполнения в секундах
    float duration = std::chrono::duration<float>(end - start).count();
    return duration;
}

struct line {
    glm::vec3 vertex1;
    glm::vec3 vertex2;
};