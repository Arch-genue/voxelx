#pragma once

// #include <iostream>
#include <cstring>
#include <string>
#include <regex>
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