#include "camera.h"
#include "../window/window.h"

#include <glm/ext.hpp>

Camera::Camera(glm::vec3 pos, float fov) : _fov(fov), _rotation(1.0f) {
    _updateVectors();
    _position = pos;
    _aspect = 0.0f;
    _perspective = true;
    _flipped = false;
    _zoom = 1.0f;
}
Camera::~Camera() {}

void Camera::_updateVectors() {
    _front = glm::vec3(_rotation * glm::vec4(0, 0, -1, 1));
    _right = glm::vec3(_rotation * glm::vec4(1, 0, 0, 1));
    _up = glm::vec3(_rotation * glm::vec4(0, 1, 0, 1));

	_dir = glm::vec3(_rotation * glm::vec4(0, 0, -1, 1));
	_dir.y = 0;
	float len = length(_dir);
	if (len > 0.0f) {
		_dir.x /= len;
		_dir.z /= len;
	}
}

void Camera::rotate(glm::vec2 rot, float z) {
    _rotation = glm::rotate(_rotation, z, glm::vec3(0,0,1));
    _rotation = glm::rotate(_rotation, rot.x, glm::vec3(0,1,0));
    _rotation = glm::rotate(_rotation, rot.y, glm::vec3(1,0,0));

    _updateVectors();
}
void Camera::setPosition(glm::vec3 pos) {
    _position = pos / 10.0f;
}
glm::vec3 Camera::getPosition() {
    return _position * 10.0f;
}


void Camera::setRotation(glm::mat4 rotation) {
    _rotation = rotation;
}
glm::mat4 Camera::getRotation() {
    return _rotation;
}


void Camera::setFOV(float fov) {
    _fov = fov;
}
float Camera::getFOV() {
    return _fov;
}


void Camera::setZoom(float zoom) {
    _zoom = zoom;
}

float Camera::getZoom() {
    return _zoom;
}


void Camera::setPerspective(bool perspective) {
    _perspective = perspective;
}

bool Camera::getPerspective() {
    return _perspective;
}


void Camera::setFlipped(bool flipped) {
    _flipped = flipped;
}

bool Camera::getFlipped() {
    return _flipped;
}


glm::vec3 Camera::getTarget() {
    return _front;
}
glm::vec3 Camera::getUp() {
    return _up;
}
glm::vec3 Camera::getRight() {
    return _right;
}


glm::mat4 Camera::getProjection() {
    float aspect = this->_aspect;
	if (aspect == 0.0f) aspect = (float)Window::width / (float)Window::height;

	if (_perspective) return glm::perspective(_fov*_zoom, aspect, 0.1f, 1500.0f);
	else {
		if (_flipped)
			return glm::ortho(0.0f, _fov*aspect, _fov, 0.0f);
		else
			return glm::ortho(0.0f, _fov*aspect, 0.0f, _fov);
    }
}

glm::mat4 Camera::getView() {
    // return glm::lookAt(position, position+front, up);
    if (_perspective) return glm::lookAt(_position/0.1f, _position/0.1f+_front, _up);
	else return glm::translate(glm::mat4(1.0f), _position);
}