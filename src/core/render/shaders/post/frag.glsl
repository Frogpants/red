#version 330 core

uniform sampler2D screenTex;

in vec2 TexCoord;
out vec4 FragColor;

void main()
{
    vec2 resolution = vec2(512.0);
    vec2 pixelSize = 1.0 / resolution;

    vec2 cell = floor(TexCoord * resolution) * pixelSize;
    vec2 center = cell + pixelSize * 0.5;

    // ---------- Base Pixel ----------
    vec2 blur = pixelSize * 0.5;

    vec4 base = vec4(0.0);
    base += texture(screenTex, center);
    base += texture(screenTex, center + vec2( blur.x, 0.0));
    base += texture(screenTex, center + vec2(-blur.x, 0.0));
    base += texture(screenTex, center + vec2(0.0,  blur.y));
    base += texture(screenTex, center + vec2(0.0, -blur.y));
    base /= 5.0;

    // ---------- Fake Bloom ----------
    vec3 bloom = vec3(0.0);

    // const float radius = 3.0;

    // for(int x=-3;x<=3;x++)
    // {
    //     for(int y=-3;y<=3;y++)
    //     {
    //         vec2 offset = vec2(x,y) * pixelSize;

    //         vec3 c = texture(screenTex, center + offset).rgb;

    //         // Extract only bright colors
    //         float bright = max(max(c.r,c.g),c.b);
    //         bright = max(bright - 0.75, 0.0);

    //         float weight = 1.0 / (1.0 + dot(vec2(x,y),vec2(x,y)));

    //         bloom += c * bright * weight;
    //     }
    // }

    // bloom *= 1.0;

    // ---------- Pixel Art Colors ----------
    const float levels = 32.0;
    base.rgb = floor(base.rgb * levels) / levels;

    float gray = dot(base.rgb, vec3(0.299,0.587,0.114));
    base.rgb = mix(vec3(gray), base.rgb, 1.1);

    base.rgb = (base.rgb - 0.5) * 1.15 + 0.5;

    // ---------- Final ----------
    base.rgb += bloom;

    FragColor = vec4(clamp(base.rgb,0.0,1.0), base.a);
}