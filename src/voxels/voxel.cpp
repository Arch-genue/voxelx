#include "voxel.h"

#include <iostream>

Voxel::Voxel() {
    setPosition(glm::vec3(0));
    setVelocity(glm::vec3(0));
    setColor(glm::vec4(1));
    setVisible(true);
}
Voxel::Voxel(glm::vec3 position) {
    setPosition(position);
    setVelocity(glm::vec3(0));
    setColor(glm::vec4(1));
    setVisible(true);
}
Voxel::Voxel(glm::vec3 position, glm::vec4 color): _position(position), _color(color) {
    setVelocity(glm::vec3(0));
    setVisible(true);
}
Voxel::Voxel(glm::vec3 position, glm::vec3 velocity, glm::vec4 color): _position(position), _velocity(velocity), _color(color) {
    setVisible(true);
}
Voxel::~Voxel() {}


/**
 * @brief Set the position of the Voxel.
 * 
 * @param position The new position of the Voxel.
 */
void Voxel::setPosition(glm::vec3 position) {
    _position = position;
}
/**
 * @brief Get the position of the Voxel
 * 
 * @return glm::vec3 
 */
glm::vec3 Voxel::getPosition() {
    return  _position;
}


/**
 * @brief Sets the velocity of the Voxel.
 * 
 * @param velocity The new velocity of the Voxel.
 */
void Voxel::setVelocity(glm::vec3 velocity) {
    _velocity = velocity;
}
/**
 * @brief Returns the velocity of the Voxel.
 * 
 * @return The velocity of the Voxel.
 */
glm::vec3 Voxel::getVelocity() {
    return _velocity;
}

void Voxel::setColor(glm::vec4 color) {
    _color = color;
}
glm::vec4 Voxel::getColor() {
    return _color;
}

void Voxel::setMaterial(Material material) {
    _material = material;
}
Material Voxel::getMaterial() {
    return _material;
}

void Voxel::setVisible(bool visible) {
    _visible = visible;
}
bool Voxel::isVisible() {
    return _visible;
}


void Voxel::setLifeTime(float lifetime) {
    _lifetime = lifetime;
}
float Voxel::getLifeTime() {
    return _lifetime;
}