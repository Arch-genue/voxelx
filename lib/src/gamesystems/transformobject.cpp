#include "transformobject.h"
#include <iostream>

TransformObject::TransformObject() {
    _modelmatrix = glm::mat4(1.0f);
    _position = glm::vec3(0);
    _rotationAngle = 0.0f;
    _rotationAxis = glm::vec3(1.0f);
    _scaling = glm::vec3(1.0f);

    _scalematrix = glm::mat4(1.0f);
    _positionmatrix = glm::mat4(1.0f);
    _rotatematrix = glm::mat4(1.0f);

    _sizes = glm::vec3(1.0f);

    _visible = true;
}
TransformObject::~TransformObject() {}

// TODO Добавить возможность вращения объекта вокруг точки
void TransformObject::update() {
    if (_visible == false) return;
    
    // _modelmatrix = glm::scale(_modelmatrix, glm::vec3(0.1f));
    _modelmatrix = glm::translate(_modelmatrix, _position);
    _modelmatrix = glm::rotate(_modelmatrix, _rotationAngle, _rotationAxis);

    // _modelmatrix = glm::translate(_modelmatrix, _position / _sizes);
    
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

void TransformObject::setPosition(glm::vec3 position) {
    _position = position;
}
glm::vec3 TransformObject::getPosition() {  
    return _position;
}

// void TransformObject::setRotationAroundPoint(float angle, glm::vec3 rotation, glm::vec3 point) {
//     _rotationAngle = angle;
//     _rotationAxis = rotation;
    
//     _rotatepoint = getPosition();
//     setPosition(point);
// }

void TransformObject::setRotation(float angle, glm::vec3 rotation) {
    _rotationAngle = angle;
    _rotationAxis = rotation;
}
void TransformObject::getRotation(float &angle, glm::vec3 &rotation) {
    angle = _rotationAngle;
    rotation = _rotationAxis;
}

void TransformObject::setScaling(glm::vec3 scaling) {
    _scaling = scaling;
}
glm::vec3 TransformObject::getScaling() {
    return _scaling;
}

void TransformObject::setSize(glm::vec3 size) {
    _sizes = size;
}
glm::vec3 TransformObject::getSize() {
    return _sizes;
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