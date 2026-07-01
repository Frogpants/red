#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 normal;

out vec4 vertexColor;
out vec2 vUV;

void main()
{
    gl_Position = vec4(position.xy, 0.0, 1.0);

    vertexColor = color;
    vUV = uv;
}