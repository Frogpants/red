#version 330 core

uniform sampler2D tex;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambient;

in vec4 vertexColor;
in vec2 vUV;
in vec3 vNormal;
in vec3 vPosition;

out vec4 FragColor;

vec2 hash2(vec2 st){
    st = vec2( dot(st,vec2(127.1,311.7)), dot(st,vec2(269.5,183.3)) );
    return fract(sin(st)*43758.5453123)*2.0 - 1.0;
}

float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    vec2 u = f*f*(f*-2.0 + 3.0);

    return mix( mix( dot( hash2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ),
                    dot( hash2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), u.x),
                mix( dot( hash2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ),
                    dot( hash2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), u.x), u.y);
}

void main()
{
    vec2 uv = vUV;
    vec2 c = uv + vPosition.xz / vec2(0.1);
    vec2 st = floor(c * 32.0) / 32.0;
    vec4 t = texture(tex, uv);
    float alpha = t.a;

    float n = noise(st * 100.0) * 0.1 + 0.9;

    vec3 N = normalize(vNormal);
    vec3 L = normalize(lightDir);

    float diff = max(dot(N, L), 0.0);
    vec3 lighting = ambient + diff * lightColor;
    lighting = clamp(lighting, 0.0, 1.0);

    vec3 col =  vertexColor.rgb * lighting * n;
    alpha *= vertexColor.a;

    FragColor = vec4(col, alpha);
}