#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec4 v_color;

out vec4 a_color;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform vec3 aPos;

out vec3 FragPos;
out vec3 Normal;

void main() {
    vec4 s_position = vec4(v_position * 0.1f, 1.0);
    // FragPos = vec3(model * vec4(d_position, 2.0));
    // FragPos = v_position;
    // FragPos = vec3(model * vec4(v_position, 5.0f));
    // Normal = mat3(transpose(inverse(model))) * v_normal;  
    // Normal = mat3(transpose(inverse(model))) * Normal;
    
    // gl_Position = projection * view * model * vec4(FragPos, 1.0);

    FragPos = vec3(model * s_position);
    Normal = v_normal;

    gl_Position = projection * view * model * s_position;
    a_color = vec4(v_color.x, v_color.y, v_color.z, v_color.w);
    
}