#pragma once

#include <glm/glm.hpp>

using namespace glm;

class Camera {
    void updateVectors();
public:
    vec3 front;
    vec3 up;
    vec3 right;

    vec3 position;
    float fov;
    mat4 rotation;
    
    Camera(vec3 pos, float fov);

    void rotate(vec2 rot, float z);

    void setPosition(vec3 pos);
    vec3 getPosition();
    mat4 getProjection();
    mat4 getView();
};