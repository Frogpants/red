#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 uv;

out vec2 TexCoord;

void main()
{
    TexCoord = uv;
    gl_Position = vec4(position.xy, 0, 1);
}