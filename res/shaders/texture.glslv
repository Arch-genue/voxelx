#version 330

layout (location = 0) in vec2 v_position;
layout (location = 1) in vec2 vertexTexCoord;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 projview;

void main()
{
    gl_Position = projview * model * vec4(v_position, 0.0, 1.0);
    texCoord = vertexTexCoord;
}