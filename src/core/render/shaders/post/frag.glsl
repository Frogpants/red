#version 330 core

uniform sampler2D screenTex;

in vec2 TexCoord;
out vec4 FragColor;

void main()
{
    vec2 resolution = vec2(512.0);
    vec2 pixelSize = 1.0 / resolution;

    vec2 cell = floor(TexCoord * resolution) / resolution;
    float aaStrength = 1.0;

    vec2 offsets[4];
    offsets[0] = vec2(0.25, 0.25);
    offsets[1] = vec2(0.75, 0.25);
    offsets[2] = vec2(0.25, 0.75);
    offsets[3] = vec2(0.75, 0.75);

    vec4 col = vec4(0.0);

    for (int i = 0; i < 4; i++)
    {
        vec2 uv = cell + offsets[i] * pixelSize;
        uv = mix(cell, uv, aaStrength);
        col += texture(screenTex, uv);
    }

    col *= 0.25;

    float alpha = col.a;
    vec3 color = col.rgb;

    FragColor = vec4(color, alpha);
}