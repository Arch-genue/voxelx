#version 330 core

in vec4 a_color;

out vec4 f_color;

void main() {
    f_color = a_color;//vec4(a_color.x*vlight, a_color.y*vlight, a_color.z*vlight, a_color.w);
}