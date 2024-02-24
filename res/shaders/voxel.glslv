#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec4 v_color;

out vec4 a_color;

uniform mat4 model;
uniform mat4 projview;

void main() {
    gl_Position = projview *  model * vec4(v_position, 2.0f);
    a_color = vec4(v_color.x, v_color.y, v_color.z, v_color.w);
}