#include "camera.h"

#include <glm/ext.hpp>

Camera::Camera(glm::vec3 position, float fov): _position(position), _fov(fov), _rotation(1.0f) {
    _updateVectors();
    _aspect = 0.0f;
    _perspective = true;
    _flipped = false;
    _zoom = 1.0f;
}

void Camera::_updateVectors() {
    _front = glm::mat3(_rotation) * glm::vec3(0,0,-1);
    _right = glm::mat3(_rotation) * glm::vec3(1,0,0);
    _up = glm::mat3(_rotation) * glm::vec3(0,1,0);

    _dir = glm::normalize(glm::vec3(_rotation * glm::vec4(0, 0, -1, 0)));
    _dir.y = 0;
    _dir = glm::normalize(_dir);
}

void Camera::rotate(const glm::vec2& rot, float z) {
    _rotation = glm::rotate(_rotation, z, glm::vec3(0,0,1));
    _rotation = glm::rotate(_rotation, rot.x, glm::vec3(0,1,0));
    _rotation = glm::rotate(_rotation, rot.y, glm::vec3(1,0,0));

    _updateVectors();
}

glm::mat4 Camera::getProjection(float aspect) const {
	if (_perspective) return glm::perspective(_fov*_zoom, aspect, 0.1f, 1500.0f);
	else {
		if (_flipped)
			return glm::ortho(0.0f, _fov * aspect, _fov, 0.0f);
		else
			return glm::ortho(0.0f, _fov * aspect, 0.0f, _fov);
    }
}

glm::mat4 Camera::getView() const {
    if (_perspective) return glm::lookAt(_position/0.1f, _position/0.1f+_front, _up);
	else return glm::translate(glm::mat4(1.0f), _position);
}