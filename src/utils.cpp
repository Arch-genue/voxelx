#include "utils.h"

void split(std::string* bufstr, std::string str, char separator) {
    uint16_t len = strlen(str.data());
    uint8_t d = 0;
    std::string s;
    for (uint8_t i = 0; i < len; i++) {
        if (str.data()[i] != separator) s += str.data()[i];
        else {
            bufstr[d] = s;
            s.clear();
            d++;
        }
    }
    bufstr[d] = s;
}

void splitvalue(std::string* bufstr, std::string str) {
    uint16_t len = strlen(str.data());
    uint8_t d = 0;
    std::string s;
    for (uint8_t i = 0; i < len; i++) {
        if (str.data()[i] == ':') continue;

        if (str.data()[i] != ' ' || d >= 1) s += str.data()[i];
        else {
            bufstr[d] = s;
            s.clear();
            d++;
        }
    }
    bufstr[d] = s;
}

bool isNumeric(const std::string& str) {
    bool hasDecimalPoint = false;
    for (char c : str) {
        if (!std::isdigit(c)) {
            if (c == '.' && !hasDecimalPoint) {
                hasDecimalPoint = true;
            } else {
                return false;
            }
        }
    }
    return true;
}

float* random_keyword_parse(std::string color) {
    // Регулярное выражение для поиска чисел в скобках после слова "random"
    // std::regex pattern("^random\\((\\d+(\\.\\d+)?),(\\d+(\\.\\d+)?)\\)$");
    std::regex pattern("^random\\((-?\\d+(\\.\\d+)?),(-?\\d+(\\.\\d+)?)\\)$");
    float* val  = new float[2];
    std::smatch match;
    if (std::regex_match(color, match, pattern)) {
        try {
            val[0] = std::stof(match[1]);
            val[1] = std::stof(match[3]);
        } catch (const std::invalid_argument&) {
            delete[] val; // Освобождаем память перед возвратом nullptr
            return nullptr;
        }
        return val;
    } else if (isNumeric(color)) {
        try {
            val[0] = std::stof(color);
            val[1] = -1500000;
        } catch (const std::invalid_argument&) {
            delete[] val;
            return nullptr;
        }
        return val;
    } else {
        delete[] val;
        return nullptr;
    }
}