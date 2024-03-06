/**
 * @file logger.h
 * @author Vlad Kartsaev
 * @brief Default logger
 * @version 1.0
 * @date 2024-03-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "utils.h"

#include <iostream>

enum class LOGLEVEL {
    ALL,
    DEBUG,
    ERROR,
    WARNING,
    INFO,
    SUCCESS
};

/**
 * @brief Класс логгирования
 * 
 */
class Logger {
private:
    static LOGLEVEL _loglevel;
public:
    static void setLogLevel(LOGLEVEL loglevel);
    static void eprint(std::string subject, std::string msg, LOGLEVEL type);
};