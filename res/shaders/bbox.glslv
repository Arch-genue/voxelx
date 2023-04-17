#version 330 core

layout (location = 0) in vec3 v_position;

uniform mat4 model;
uniform mat4 projview;

void main() {
    gl_Position = projview *  model * vec4(v_position, 1.0);
    //gl_Position = vec4(v_position, 1.0);
}