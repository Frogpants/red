#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 normal;


uniform vec3 cam;
uniform vec3 rot;
uniform float f;

uniform vec3 modelPos;
uniform vec3 modelRot;
uniform vec3 modelScale;
uniform vec4 modelColor;


out vec3 vNormal;
out vec4 vertexColor;
out vec2 vUV;


vec3 transformModel(vec3 p) {
    p *= modelScale;

    vec3 r = radians(modelRot);

    float x = p.x;
    float z = p.z;
    p.x = x * cos(r.x) - z * sin(r.x);
    p.z = x * sin(r.x) + z * cos(r.x);

    float y = p.y;
    z = p.z;
    p.y = y * cos(r.y) + z * sin(r.y);
    p.z = -y * sin(r.y) + z * cos(r.y);

    x = p.x;
    y = p.y;
    p.x = x * cos(r.z) - y * sin(r.z);
    p.y = x * sin(r.z) + y * cos(r.z);

    p += modelPos;

    return p;
}

vec3 project() {
    vec3 p = transformModel(position);
    p = p - cam;

    vec3 trig = radians(rot);

    float tx = p.x;
    float tz = p.z;

    p.x = tx * cos(trig.x) - tz * sin(trig.x);
    p.z = tx * sin(trig.x) + tz * cos(trig.x);

    float ty = p.y;
    tz = p.z;

    p.y = ty * cos(trig.y) - tz * sin(trig.y);
    p.z = ty * sin(trig.y) + tz * cos(trig.y);

    tx = p.x;
    ty = p.y;

    p.x = tx * cos(trig.z) - ty * sin(trig.z);
    p.y = tx * sin(trig.z) + ty * cos(trig.z);

    // p.z = max(p.z, 0.1);

    // p.xy = clamp(p.xy, vec2(-1.0), vec2(1.0));

    float near = 0.01;
    float far  = 1.0;


    return vec3(p.xy * f, -p.z);
}

void main()
{
    gl_Position = vec4(project(), 1.0);

    vertexColor = color * modelColor;
    vNormal = normal;

    vUV = uv;
}