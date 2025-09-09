#pragma once

#include <glm/glm.hpp>

struct Color { // 4 байта
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;

    bool operator==(const Color& clr) {
        return r == clr.r and g == clr.g and b == clr.b and a == clr.a;
    }
    bool operator!=(const Color& clr) {
        return r != clr.r or g != clr.g or b != clr.b or a != clr.a;
    }
};

struct Voxel {
    Color color = {0, 0, 0, 0}; // r=0, g=0, b=0, a=0;
    uint8_t mat : 4 = 0x1; // 0..15 типов
    uint8_t visible : 1 = 0;
    uint8_t unused : 3; // 3 бита — для выравнивания

    // Геттер для glm::vec4
    glm::vec4 getColor() const {
        float clr_r = color.r / 255.0f;
        float clr_g = color.g / 255.0f;
        float clr_b = color.b / 255.0f;
        float clr_a = color.a / 255.0f;
        return glm::vec4(clr_r, clr_g, clr_b, clr_a);
    }

    Color& getColor() {
        return color;
    }

    // Сеттер из glm::vec4
    void setColor(const glm::vec4& clr) {
        color.r = static_cast<uint8_t>(glm::clamp(clr.r, 0.0f, 1.0f) * 255.0f);
        color.g = static_cast<uint8_t>(glm::clamp(clr.g, 0.0f, 1.0f) * 255.0f);
        color.b = static_cast<uint8_t>(glm::clamp(clr.b, 0.0f, 1.0f) * 255.0f);
        color.a = static_cast<uint8_t>(glm::clamp(clr.a, 0.0f, 1.0f) * 255.0f);
    }
    void setColor(uint8_t clr_r, uint8_t clr_g, uint8_t clr_b, uint8_t clr_a) {
        color.r = clr_r;
        color.g = clr_g;
        color.b = clr_b;
        color.a = clr_a;
    }
};