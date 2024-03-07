#include "voxel.h"

Voxel::Voxel() {
    setPosition(glm::vec3(0));
    setColor(glm::vec4(1));
    setVisible(true);
}
Voxel::Voxel(glm::ivec3 position) {
    setPosition(position);
    setColor(glm::vec4(1));
    setVisible(true);
}
Voxel::Voxel(glm::ivec3 position, glm::vec4 color): _position(position), _color(color) {
    setVisible(true);
}

Voxel::~Voxel() {}

/**
 * @brief Set the position of the Voxel.
 * 
 * @param position The new position of the Voxel.
 */
void Voxel::setPosition(glm::ivec3 position) {
    _position = position;
}
/**
 * @brief Get the position of the Voxel
 * 
 * @return glm::vec3 
 */
glm::ivec3 Voxel::getPosition() {
    return  _position;
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