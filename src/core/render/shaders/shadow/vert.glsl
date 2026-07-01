#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 normal;

uniform vec3 modelPos;
uniform vec3 modelRot;
uniform vec3 modelScale;

uniform vec3 shadowOrigin;
uniform vec3 shadowRight;
uniform vec3 shadowUp;
uniform vec3 shadowForward;
uniform float shadowRange;

out float vDepth;

vec3 rotateModel(vec3 p)
{
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

    return p;
}

vec3 transformModel(vec3 p)
{
    p *= modelScale;
    p = rotateModel(p);
    p += modelPos;

    return p;
}

void main()
{
    vec3 worldPos = transformModel(position);
    vec3 rel = worldPos - shadowOrigin;

    float sx = dot(rel, shadowRight) / shadowRange;
    float sy = dot(rel, shadowUp) / shadowRange;
    float sz = dot(rel, shadowForward) / shadowRange;

    gl_Position = vec4(sx, sy, sz, 1.0);
    vDepth = sz * 0.5 + 0.5;
}