#version 330 core

uniform sampler2D sceneTex;
uniform sampler2D uiTex;
uniform int hasUi;

in vec2 TexCoord;
out vec4 FragColor;

void main()
{
    vec2 resolution = vec2(256.0);
    vec2 pixelSize = 1.0 / resolution;

    vec2 cell = floor(TexCoord * resolution) * pixelSize;
    vec2 center = cell + pixelSize * 0.5;

    vec2 blur = pixelSize * 0.3;

    vec4 base = vec4(0.0);
    base += texture(sceneTex, center);
    base += texture(sceneTex, center + vec2( blur.x, 0.0));
    base += texture(sceneTex, center + vec2(-blur.x, 0.0));
    base += texture(sceneTex, center + vec2(0.0,  blur.y));
    base += texture(sceneTex, center + vec2(0.0, -blur.y));
    base /= 5.0;

    vec3 bloom = vec3(0.0);

    const float levels = 32.0;
    base.rgb = floor(base.rgb * levels) / levels;

    float gray = dot(base.rgb, vec3(0.299,0.587,0.114));
    base.rgb = mix(vec3(gray), base.rgb, 1.1);

    base.rgb = (base.rgb - 0.5) * 1.15 + 0.5;

    base.rgb += bloom;

    vec4 ui = hasUi != 0 ? texture(uiTex, TexCoord) : vec4(0.0);
    vec3 outRgb = mix(base.rgb, ui.rgb, ui.a);
    float outAlpha = max(base.a, ui.a);

    FragColor = vec4(clamp(outRgb, 0.0, 1.0), outAlpha);
}