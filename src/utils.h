#pragma once

// #include <iostream>
#include <cstring>
#include <string>
#include <regex>
#include <chrono>
#include <glm/glm.hpp>


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
float measureFunctionTime(Func func, Args&&... args) {
    auto start = std::chrono::steady_clock::now(); // Засекаем начало времени

    // Вызываем переданную функцию с переданными аргументами
    func(std::forward<Args>(args)...);

    auto end = std::chrono::steady_clock::now(); // Засекаем конец времени

    // Вычисляем время выполнения в секундах
    float duration = std::chrono::duration<float>(end - start).count();
    return duration;
}