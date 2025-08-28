#include "transformobject.h"
#include <iostream>

TransformObject::TransformObject() {
    _modelmatrix = glm::mat4(1.0f);
    _position = glm::vec3(0);

    _positionmatrix = glm::mat4(1.0f);
    _rotationMatrix = glm::mat4(1.0f);
}
TransformObject::~TransformObject() {}

// TODO Добавить возможность вращения объекта вокруг точки
void TransformObject::update() {
    _modelmatrix = glm::translate(_modelmatrix, _position);
    _modelmatrix *= _rotationMatrix;

    onTransformed();
    _modelmatrix = glm::mat4(1.0f);
}
void TransformObject::onTransformed() {}

void TransformObject::translate(float value, glm::vec3 vector) {
	_position += vector * value;
}

void TransformObject::rotate(float angle, glm::vec3 vector) {
    // _rotationAngle += angle;
    // _rotationAxis = vector; 
}

void TransformObject::setPosition(glm::vec3 position) {
    _position = position;
}
glm::vec3 TransformObject::getPosition() {  
    return _position;
}

void TransformObject::setRotationMat(glm::mat4 rotation) {
    _rotationMatrix = rotation;
}

void TransformObject::setRotation(float angle, glm::vec3 rotation) {
    // _rotationAngle = angle;
    // _rotationAxis = rotation;
}
void TransformObject::getRotation(float &angle, glm::vec3 &rotation) {
    // angle = _rotationAngle;
    // rotation = _rotationAxis;
}

// void TransformObject::setVisible(bool visible) {
//     _visible = visible;
// }
// bool TransformObject::isVisible() {
//     return _visible;
// }

glm::mat4 TransformObject::getMatrix() {
    return _modelmatrix;
}