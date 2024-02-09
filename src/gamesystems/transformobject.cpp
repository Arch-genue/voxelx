#include "transformobject.h"
#include <iostream>

TransformObject::TransformObject() {
    _modelmatrix = glm::mat4(1.0f);
    _position = glm::vec3(0);
    _rotationAngle = 0.0f;
    _rotationAxis = glm::vec3(1);
    _scaling = glm::vec3(0.1f);

    _visible = true;
}
TransformObject::~TransformObject() {}

void TransformObject::update() {
    if (_visible == false) return;
    // setMatrix();
    _modelmatrix = glm::scale(_modelmatrix, _scaling);
    _modelmatrix = glm::translate(_modelmatrix, _position);
	// std::cout << "position: " << getPosition().x << ", " << getPosition().y << ", " << getPosition().z << std::endl;
    draw();
    _modelmatrix = glm::mat4(1.0f);
}
void TransformObject::draw() {}

void TransformObject::translate(float value, glm::vec3 vector) {
	_position += vector * value;
}

void TransformObject::rotate(float angle, glm::vec3 vector) {
    _rotationAngle += angle;
    _rotationAxis = vector; 
}

void TransformObject::setPosition(glm::vec3 position)
{
    _position = position;
}
glm::vec3 TransformObject::getPosition() {  
    return _position;
}

void TransformObject::setRotation(float angle, glm::vec3 rotation) {
    _rotationAngle = angle;
    _rotationAxis = rotation; 
    glm::mat4 rotationmatrix = glm::rotate(_modelmatrix, _rotationAngle, _rotationAxis);

    _modelmatrix = rotationmatrix * _modelmatrix;
}
void TransformObject::getRotation(float &angle, glm::vec3 &rotation) {
    angle = _rotationAngle;
    rotation = _rotationAxis;
}

void TransformObject::setVisible(bool visible) {
    _visible = visible;
}
bool TransformObject::isVisible() {
    return _visible;
}

glm::mat4 TransformObject::getMatrix() {
    return _modelmatrix;
}