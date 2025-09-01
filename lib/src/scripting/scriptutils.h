#pragma once

#include <glm/glm.hpp>

class ScriptUtils {
public:
    static glm::vec3 get_vec3(float x, float y, float z) { return glm::vec3(x, y, z); }
    static glm::vec3 get_vec3(float val) { return glm::vec3(val); }
    static glm::vec4 get_vec4(float x, float y, float z, float a) { return glm::vec4(x, y, z, a); }
};