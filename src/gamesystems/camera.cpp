#include "camera.h"
#include "../window/window.h"

#include <glm/ext.hpp>

Camera::Camera(vec3 pos, float fov) : fov(fov), rotation(1.0f) {
    updateVectors();
    position = pos;
    aspect = 0.0f;
    perspective = true;
    flipped = false;
    zoom = 1.0f;
}

void Camera::updateVectors() {
    front = vec3(rotation * vec4(0, 0, -1, 1));
    right = vec3(rotation * vec4(1, 0, 0, 1));
    up = vec3(rotation * vec4(0, 1, 0, 1));

	dir = vec3(rotation * vec4(0, 0, -1, 1));
	dir.y = 0;
	float len = length(dir);
	if (len > 0.0f) {
		dir.x /= len;
		dir.z /= len;
	}
}

void Camera::rotate(vec2 rot, float z) {
    rotation = glm::rotate(rotation, z, vec3(0,0,1));
    rotation = glm::rotate(rotation, rot.x, vec3(0,1,0));
    rotation = glm::rotate(rotation, rot.y, vec3(1,0,0));

    updateVectors();
}
void Camera::setPosition(vec3 pos) {
    position = pos;
}
vec3 Camera::getPosition() {
    return position*10.0f;
}

mat4 Camera::getProjection() {
    // float aspect = (float)Window::width / (float)Window::height;
    
    // return glm::perspective(fov, aspect, 0.1f, 100.0f);
    float aspect = this->aspect;
	if (aspect == 0.0f) aspect = (float)Window::width / (float)Window::height;

	if (perspective) return glm::perspective(fov*zoom, aspect, 0.05f, 1500.0f);
	else {
		if (flipped)
			return glm::ortho(0.0f, fov*aspect, fov, 0.0f);
		else
			return glm::ortho(0.0f, fov*aspect, 0.0f, fov);
    }
}

mat4 Camera::getView() {
    // return glm::lookAt(position, position+front, up);
    if (perspective) return glm::lookAt(position, position+front, up);
	else return glm::translate(glm::mat4(1.0f), position);
}