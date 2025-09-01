/**
 * @file logger.hpp
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
#include <sstream>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <unordered_map>

enum class LogLevel : uint8_t {
    VERBOSE = 0,
    DEBUG = 1,
    ERROR = 2,
    WARNING = 3,
    INFO = 4,
    SUCCESS = 5
};

// Цвета ANSI
namespace LogColor {
    constexpr const char* RESET  = "\033[0m";

    constexpr const char* BLACK  = "\033[0;30m";
    constexpr const char* RED    = "\033[1;31m";
    constexpr const char* GREEN  = "\033[1;32m";
    constexpr const char* YELLOW = "\033[1;33m";
    constexpr const char* CYAN   = "\033[1;36m";
    constexpr const char* WHITE  = "\033[1;37m";
    constexpr const char* BLUE   = "\033[0;34m";
    constexpr const char* MAGENTA= "\033[0;35m";

    constexpr const char* BRIGHT_BLACK              = "\033[1;30m";
    constexpr const char* BRIGHT_RED                = "\033[1;91m";
    constexpr const char* BRIGHT_GREEN              = "\033[1;32m";
    constexpr const char* BRIGHT_YELLOW             = "\033[1;33m";
    constexpr const char* BRIGHT_CYAN               = "\033[1;36m";
    constexpr const char* BRIGHT_WHITE              = "\033[1;37m";
    constexpr const char* BRIGHT_BLUE               = "\033[1;34m";
    constexpr const char* BRIGHT_MAGENTA            = "\033[1;35m";
    ;
    constexpr const char* BLACK_BACKGROUND          = "\033[40m";
    constexpr const char* WHITE_BACKGROUND          = "\033[107m";
    constexpr const char* BRIGHT_YELLOW_BACKGROUND  = "\033[103m";
    constexpr const char* BACKGROUND_RED            = "\033[1;41m";
    constexpr const char* TEXT_BOLD                 = "\033[1m";
    constexpr const char* TEXT_UNDERLINE            = "\033[4m";
    constexpr const char* TEXT_ITALIC               = "\033[3m";
    constexpr const char* TEXT_INVERSE              = "\033[7m";
    constexpr const char* TEXT_FAIL                 = "\033[91m";
}

inline const std::unordered_map<std::string, const char*> colorMap = {
    {"black", LogColor::BLACK},
    {"red", LogColor::RED},
    {"green", LogColor::GREEN},
    {"yellow", LogColor::YELLOW},
    {"cyan", LogColor::CYAN},
    {"white", LogColor::WHITE},
    {"blue", LogColor::BLUE},
    {"magenta", LogColor::MAGENTA},

    {"brblack", LogColor::BRIGHT_BLACK},
    {"brred", LogColor::BRIGHT_RED},
    {"brgreen", LogColor::BRIGHT_GREEN},
    {"bryellow", LogColor::BRIGHT_YELLOW},
    {"brcyan", LogColor::BRIGHT_CYAN},
    {"brwhite", LogColor::BRIGHT_WHITE},
    {"brblue", LogColor::BRIGHT_BLUE},
    {"brmagenta", LogColor::BRIGHT_MAGENTA},

    {"backblack", LogColor::BLACK_BACKGROUND},
    {"backwhite", LogColor::WHITE_BACKGROUND},
    {"brbackyellow", LogColor::BRIGHT_YELLOW_BACKGROUND},
    {"backred", LogColor::BACKGROUND_RED},

    {"txtbold", LogColor::TEXT_BOLD},
    {"txtunder", LogColor::TEXT_UNDERLINE},
    {"txtitalic", LogColor::TEXT_ITALIC},
    {"txtinverse", LogColor::TEXT_INVERSE},
    {"txtfail", LogColor::TEXT_FAIL}
};

/**
 * @brief Класс логгирования
 * 
 */
class Logger {
public:
    // Singleton
    static Logger& instance() {
        static Logger inst;
        return inst;
    }

    void setLogLevel(LogLevel level) { _level = level; }

    template<typename... Args>
    void log(LogLevel type, const std::string& subject, Args&&... args) {
        if (type < _level) return;

        std::ostringstream oss;
        (oss << ... << args); // fold expression C++17

        std::lock_guard<std::mutex> lock(_mutex);

        std::string levelStr;
        const char* color = LogColor::WHITE;
        switch(type) {
            case LogLevel::ERROR:   levelStr = "ERROR";   color = LogColor::RED;    break;
            case LogLevel::WARNING: levelStr = "WARNING"; color = LogColor::YELLOW; break;
            case LogLevel::INFO:    levelStr = "INFO";    color = LogColor::CYAN;   break;
            case LogLevel::SUCCESS: levelStr = "SUCCESS"; color = LogColor::GREEN; break;
            case LogLevel::DEBUG:   levelStr = "DEBUG";   color = LogColor::WHITE;  break;
            case LogLevel::VERBOSE: levelStr = "VERBOSE"; color = LogColor::MAGENTA;  break;
        }

        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm tm_buf;
        localtime_r(&time, &tm_buf);

        std::string mainstr = oss.str();
        std::string replaced = this->replaceColors(mainstr);

        std::ostringstream header;
        header << color
               << "[" << levelStr << "] "
               << "[" << subject << "] "
               << std::put_time(&tm_buf, "%H:%M:%S")
               << LogColor::RESET << " ";

        std::cout << header.str() << replaced << std::endl;
    }

private:
    Logger() = default;
    ~Logger() = default;

    LogLevel _level = LogLevel::DEBUG;
    std::mutex _mutex;

    std::string replaceColors(const std::string& input) {
        std::string result;
        size_t pos = 0;
        while (pos < input.size()) {
            size_t start = input.find('<', pos);
            if (start == std::string::npos) {
                result += input.substr(pos);
                break;
            }
            // ищем пробел или конец префикса перед <
            size_t prefixStart = input.rfind(' ', start);
            if (prefixStart == std::string::npos) prefixStart = 0;
            else prefixStart += 1;

            std::string prefix = input.substr(prefixStart, start - prefixStart);
            std::string key = prefix; // используем префикс для colorMap

            auto it = colorMap.find(key);
            const char* color = (it != colorMap.end()) ? it->second : "";

            result += input.substr(pos, prefixStart - pos); // текст до префикса
            result += color;                                // цвет
            result += input.substr(start + 1, input.find('>', start) - start - 1); // текст внутри <>
            result += LogColor::RESET;

            pos = input.find('>', start) + 1;
        }
        return result;
    }
};