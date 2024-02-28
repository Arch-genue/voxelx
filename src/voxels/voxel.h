/**
 * @file voxel.h
 * @author Vlad Kartsaev
 * @brief Implementation of the  Voxel class.
 * @version 2.0
 * @date 2023-04-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "../utils.h"
#include <vector>

#include "../graphics/types/material.h"

/**
 * @brief Класс описывающий воксель
 * 
 */
class Voxel {
private:
    glm::vec3 _position;
    glm::vec3 _velocity;
    glm::vec4 _color;

    Material _material;

    float _lifetime;

    bool _visible;
public:
    Voxel();
    Voxel(glm::vec3 position);
    Voxel(glm::vec3 position, glm::vec4 color);
    Voxel(glm::vec3 position, glm::vec3 velocity, glm::vec4 color);

    ~Voxel();

    void setPosition(glm::vec3 position);

    /**
     * @brief Get the Position object
     * 
     * @return glm::vec3 
     */
    glm::vec3 getPosition();

    void setVelocity(glm::vec3 velocity);
    glm::vec3 getVelocity();

    void setColor(glm::vec4 color);
    glm::vec4 getColor();

    void setMaterial(Material material);
    Material getMaterial();

    void setVisible(bool visible);
    bool isVisible();

    void setLifeTime(float lifetime);
    float getLifeTime();
};
