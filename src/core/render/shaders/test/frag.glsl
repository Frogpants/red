#version 330 core

in vec4 vertexColor;
in vec2 vUV;

out vec4 FragColor;

void main() {
    vec2 uv = vUV;
    vec3 col = vec3(sin(uv.x * 2.0), cos(uv.y), sin(uv.x * uv.y));

    FragColor = vertexColor;
}