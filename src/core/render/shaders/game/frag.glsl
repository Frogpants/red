#version 330 core

in vec4 vertexColor;
in vec2 vUV;

out vec4 FragColor;

void main()
{
    vec2 uv = vUV;
    vec2 st = floor(uv * 16.0) / 16.0;
    vec3 col = vec3(sin(st.x * 2.0), cos(st.y), sin(st.x * st.y));

    FragColor = vec4(col, 1.0) * vertexColor;
}