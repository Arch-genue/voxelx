#pragma once

#include <string>
#include <glm/glm.hpp>

#include "types/material.h"

class Shader {
public:
    unsigned int id;
    
    Shader(unsigned int id);
    ~Shader();

    void use();
    void uniformMatrix(const std::string& name, const glm::mat4& matrix);
    void uniformFloat(const std::string& name, float val);
    void uniformVec3(const std::string& name, const glm::vec3& var);
    
    //* Special functions */
    void uniformLight(const std::string& name, Light &light);
    void uniformMaterial(const std::string& name, Material &material);
    void uniformLightPosition(const std::string& name, glm::vec3 var);
};
extern Shader * load_shader(const std::string& vertexFile, const std::string& fragmentFile); 