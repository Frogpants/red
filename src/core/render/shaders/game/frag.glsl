#version 330 core

in vec4 vertexColor;
in vec2 vUV;

uniform sampler2D tex;

out vec4 FragColor;

void main() {
    gl_FragColor = vertexColor;
    vec2 uv = vUV;

    FragColor = texture(tex, uv);
}