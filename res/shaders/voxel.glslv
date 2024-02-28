#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 d_position;
layout (location = 2) in vec3 v_normal;
layout (location = 3) in vec4 v_color;

out vec4 a_color;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform vec3 aPos;       // Позиция вершины

out vec3 FragPos;   // Передача позиции фрагмента
out vec3 Normal;    // Передача нормали фрагмента

void main() {
    // FragPos = vec3(model * vec4(d_position, 2.0));
    // FragPos = v_position;
    // FragPos = vec3(model * vec4(v_position, 5.0f));
    // Normal = mat3(transpose(inverse(model))) * v_normal;  
    // Normal = mat3(transpose(inverse(model))) * Normal;
    
    // gl_Position = projection * view * model * vec4(FragPos, 1.0);

    gl_Position = projection * view *  model * vec4(v_position, 1.0f);
    FragPos = vec3(model * vec4(d_position, 1.0f));
    a_color = vec4(v_color.x, v_color.y, v_color.z, v_color.w);
    Normal = mat3(transpose(inverse(model))) * v_normal;
}