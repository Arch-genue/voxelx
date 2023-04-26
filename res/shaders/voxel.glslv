#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec4 v_color;
layout (location = 2) in vec3 v_light;

out vec4 a_color;

uniform mat4 model;
uniform mat4 projview;

void main() {
    gl_Position = projview *  model * vec4(v_position, 1.0);
    a_color = vec4(v_color.x*v_light.x, v_color.y*v_light.y, v_color.z*v_light.z, v_color.w);
}