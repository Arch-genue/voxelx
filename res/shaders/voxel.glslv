#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec4 vcolor;
layout (location = 2) in float vlight;

out vec4 a_color;

uniform mat4 model;
uniform mat4 projview;

uniform float vtime;

void main() {
    gl_Position = projview *  model * vec4(v_position, 1.0);
    a_color = vec4(vcolor.x*vlight, vcolor.y*vlight, vcolor.z*vlight, vcolor.w);
}