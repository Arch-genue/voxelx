#pragma once

#include <cstring>
#include <string>
#include <glm/glm.hpp>


static void split(std::string* bufstr, std::string str, char separator) {
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

static void splitvalue(std::string* bufstr, std::string str) {
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