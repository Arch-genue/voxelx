#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec4 v_color;

out vec4 a_color;

uniform mat4 projviewmodel;

void main() {
    gl_Position = projviewmodel * vec4(v_position, 1.0);
    a_color = v_color;
}