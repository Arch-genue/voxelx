#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec4 vcolor;
layout (location = 2) in float vlight;

out vec4 a_color;

uniform mat4 model;
uniform mat4 projview;

void main() {
    a_color = vcolor;//vec4(vcolor.x, vcolor.y, vcolor.z, vcolor.w);;//vec4(vcolor.x, vcolor.y, vcolor.z, 0.5f);  // v_color;
    gl_Position = projview * model * vec4(v_position, 1.0);
}