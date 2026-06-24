#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 uv;

in uniform vec3 cam;
in uniform vec3 rot;
in uniform float f;

out vec4 vertexColor;
out vec2 vUV;

void rotate(vec3 p, vec3 r, vec3 dir) {
    
}

void project() {
    vec3 p = position - cam;
    
}

void main()
{
    gl_Position = vec4(position, 1.0);

    vertexColor = color;

    vUV = uv;
}