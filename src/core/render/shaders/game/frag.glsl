#version 330 core

uniform sampler2D tex;
uniform sampler2D lightTex;
uniform sampler2D shadowTex;

uniform vec3 cameraPos;
uniform vec3 ambient;
uniform vec3 worldLightDir;
uniform vec3 worldLightColor;
uniform float worldLightIntensity;
uniform vec3 shadowOrigin;
uniform vec3 shadowRight;
uniform vec3 shadowUp;
uniform vec3 shadowForward;
uniform float shadowRange;
uniform float shadowBias;
uniform int lightCount;

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

struct LightData {
    vec3 position;
    float radius;
    vec3 color;
    float intensity;
};

LightData readLight(int index) {
    int baseX = index * 2;

    vec4 a = texelFetch(lightTex, ivec2(baseX, 0), 0);
    vec4 b = texelFetch(lightTex, ivec2(baseX + 1, 0), 0);

    LightData light;
    light.position = a.xyz;
    light.radius = a.w;
    light.color = b.xyz;
    light.intensity = b.w;

    return light;
}

float softShadow(vec3 worldPos, vec3 normal, vec3 lightPos, float lightRadius) {
    vec3 toLight = lightPos - worldPos;
    float dist = length(toLight);
    vec3 L = toLight / max(dist, 0.0001);

    float ndl = max(dot(normal, L), 0.0);
    float facingShadow = smoothstep(0.08, 0.92, 1.0 - ndl);
    float heightShadow = smoothstep(-0.35, 0.45, lightPos.y - worldPos.y);
    float distanceShadow = smoothstep(0.0, lightRadius, dist);

    return 1.0 - 0.55 * facingShadow * heightShadow * distanceShadow;
}

vec3 evaluatePointLight(vec3 worldPos, vec3 normal, vec3 viewDir, LightData light) {
    vec3 toLight = light.position - worldPos;
    float dist = length(toLight);
    vec3 L = toLight / max(dist, 0.0001);

    float attenuation = 1.0 / (1.0 + 0.12 * dist + 0.04 * dist * dist);
    attenuation *= smoothstep(light.radius, 0.0, dist);

    float ndl = max(dot(normal, L), 0.0);
    float shadow = softShadow(worldPos, normal, light.position, light.radius);

    vec3 halfDir = normalize(L + viewDir);
    float spec = pow(max(dot(normal, halfDir), 0.0), 28.0);

    vec3 diffuse = light.color * ndl * attenuation * shadow * light.intensity;
    vec3 specular = light.color * spec * attenuation * shadow * 0.35 * light.intensity;

    return diffuse + specular;
}

vec3 evaluateWorldLight(vec3 worldPos, vec3 normal, vec3 viewDir) {
    vec3 L = normalize(-worldLightDir);
    float ndl = max(dot(normal, L), 0.0);
    float skyFacing = smoothstep(-0.2, 0.9, normal.y);

    vec3 rel = worldPos - shadowOrigin;
    vec2 shadowUV = vec2(dot(rel, shadowRight), dot(rel, shadowUp)) / shadowRange * 0.5 + 0.5;
    float shadowDepth = dot(rel, shadowForward) / shadowRange * 0.5 + 0.5;

    float shadow = 1.0;
    if (shadowUV.x >= 0.0 && shadowUV.x <= 1.0 && shadowUV.y >= 0.0 && shadowUV.y <= 1.0) {
        float mapDepth = texture(shadowTex, shadowUV).r;
        shadow = shadowDepth - shadowBias <= mapDepth ? 1.0 : 0.25;
    }

    shadow *= mix(0.45, 1.0, skyFacing) * mix(0.55, 1.0, ndl);

    vec3 halfDir = normalize(L + viewDir);
    float spec = pow(max(dot(normal, halfDir), 0.0), 18.0);

    vec3 diffuse = worldLightColor * ndl * worldLightIntensity * shadow;
    vec3 specular = worldLightColor * spec * 0.2 * worldLightIntensity * shadow;

    return diffuse + specular;
}

void main()
{
    vec2 uv = vUV;
    vec2 c = uv + vPosition.xz / vec2(0.1);
    vec2 st = floor(c * 16.0) / 16.0;
    vec4 t = texture(tex, uv);
    float alpha = t.a;

    float n = noise(st * 100.0) * 0.1 + 0.9;

    vec3 N = normalize(vNormal);
    vec3 V = normalize(cameraPos - vPosition);

    float cavity = mix(0.7, 1.0, clamp(N.y * 0.5 + 0.5, 0.0, 1.0));

    vec3 lighting = ambient;
    lighting += evaluateWorldLight(vPosition, N, V);

    for (int i = 0; i < lightCount; ++i) {
        LightData light = readLight(i);
        lighting += evaluatePointLight(vPosition, N, V, light);
    }

    lighting *= cavity;
    lighting = clamp(lighting, 0.0, 1.0);

    vec3 col = vertexColor.rgb * lighting * n;
    alpha *= vertexColor.a;

    FragColor = vec4(col, alpha);
}