#pragma once

#include <glm/glm.hpp>

using namespace glm;

class Camera {
    void updateVectors();
public:
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 dir;

    vec3 position;
    float fov;
    float zoom;
    mat4 rotation;
	bool perspective = true;
	bool flipped = false;
	float aspect = 0.0f;

    Camera(vec3 pos, float fov);

    void rotate(vec2 rot, float z);

    void setPosition(vec3 pos);
    vec3 getPosition();

    mat4 getProjection();
    mat4 getView();
};