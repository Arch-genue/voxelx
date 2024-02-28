/**
 * @file material.h
 * @author Vlad Kartsaev
 * @brief Material structure
 * @version 0.1
 * @date 2024-02-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <glm/glm.hpp>

struct Material {
    glm::vec3 ambient;       // Окружающая составляющая освещения
    glm::vec3 diffuse;       // Диффузная составляющая освещения
    glm::vec3 specular;      // Зеркальная составляющая освещения
    float shininess;    // Коэффициент блеска
};

struct Light {
    glm::vec3 position;  
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;
  
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};