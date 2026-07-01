#version 330 core

in float vDepth;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vDepth, vDepth, vDepth, 1.0);
}