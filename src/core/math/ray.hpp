#pragma once

#include "essentials.hpp"

#include "../render/camera.hpp"

struct Ray {
    vec3 origin;
    vec3 direction;
};

vec3 inverseRotate(vec3 p, vec3 rot)
{
    rot = radians(rot);

    // Undo Z
    {
        float tx = p.x;
        float ty = p.y;

        p.x = tx * cos(-rot.z) - ty * sin(-rot.z);
        p.y = tx * sin(-rot.z) + ty * cos(-rot.z);
    }

    // Undo Y
    {
        float ty = p.y;
        float tz = p.z;

        p.y = ty * cos(-rot.y) - tz * sin(-rot.y);
        p.z = ty * sin(-rot.y) + tz * cos(-rot.y);
    }

    // Undo X
    {
        float tx = p.x;
        float tz = p.z;

        p.x = tx * cos(-rot.x) - tz * sin(-rot.x);
        p.z = tx * sin(-rot.x) + tz * cos(-rot.x);
    }

    return p;
}

Ray mouseRay(vec2 mouseNDC, Camera& cam) {
    Ray ray;

    vec3 local(mouseNDC.x / cam.focal, mouseNDC.y / cam.focal, -1.0f);

    ray.origin = cam.pos;
    ray.direction = normalize(inverseRotate(local, cam.rot));
    ray.direction = normalize(ray.direction);

    return ray;
}

bool rayTriangle(const Ray& ray, vec3 a, vec3 b, vec3 c, float& t) {
    const float EPS = 0.000001f;

    vec3 edge1 = b - a;
    vec3 edge2 = c - a;

    vec3 h = cross(ray.direction, edge2);

    float det = dot(edge1, h);

    if (fabs(det) < EPS) {
        return false;
    }

    float invDet = 1.0f / det;

    vec3 s = ray.origin - a;

    float u = dot(s, h) * invDet;

    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    vec3 q = cross(s, edge1);

    float v = dot(ray.direction, q) * invDet;

    if (v < 0.0f || u + v > 1.0f) {
        return false;
    }

    t = dot(edge2, q) * invDet;

    return t >= EPS;
}