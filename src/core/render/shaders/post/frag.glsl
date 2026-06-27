#version 330 core

uniform sampler2D screenTex;

in vec2 TexCoord;
out vec4 FragColor;

void main()
{
    vec2 resolution = vec2(256.0);

    // size of one pixel in UV space
    vec2 pixelSize = 1.0 / resolution;

    // base pixel cell
    vec2 cell = floor(TexCoord * resolution) / resolution;

    // jitter offsets inside the pixel cell
    vec2 offsets[4];
    offsets[0] = vec2(0.25, 0.25);
    offsets[1] = vec2(0.75, 0.25);
    offsets[2] = vec2(0.25, 0.75);
    offsets[3] = vec2(0.75, 0.75);

    vec4 col = vec4(0.0);

    for (int i = 0; i < 4; i++)
    {
        vec2 uv = cell + offsets[i] * pixelSize;
        col += texture(screenTex, uv);
    }

    col *= 0.25;

    float alpha = col.a;
    vec3 color = col.rgb;

    // color = floor(color * 16.0) / 16.0;
    // color *= vec3(1.0, 0.8, 0.9);

    FragColor = vec4(color, alpha);
}