#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 uv;

uniform vec3 cam;
uniform vec3 rot;
uniform float f;

out vec4 vertexColor;
out vec2 vUV;


vec2 project() {
    vec3 p = position - cam;

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

    return p.xy * f;
}

void main()
{
    gl_Position = vec4(project(), 0.0, 1.0);

    vertexColor = color;

    vUV = uv;
}