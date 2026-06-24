#version 330 core

in vec4 vertexColor;
in vec2 vUV;

out vec4 FragColor;

void main()
{
    FragColor = vertexColor;
}