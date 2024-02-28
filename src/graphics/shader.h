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
    void uniformMatrix(std::string name, glm::mat4 matrix);
    void uniformFloat(std::string name, float val);
    void uniformVec3(std::string name, glm::vec3 var);
    
    //*Special functions*/
    void uniformLight(std::string name, Light &light);
    void uniformMaterial(std::string name, Material &material);
    void uniformLightPosition(std::string name, glm::vec3 var);
};
extern Shader * load_shader(std::string vertexFile, std::string fragmentFile); 