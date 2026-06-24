#pragma once

#include "../math/essentials.hpp"

struct Camera {
    vec3 pos;
    vec3 rot = vec3(45.0, 35.264, 0.0);

    float FOV = 90.0;
    float focal = 1.0;

    void follow(vec3 p, float speed) {
        pos = pos + (p - pos) * speed;
    }

    void follow(vec3 p) {
        pos = p;
    }

    void follow(float speed) {
        pos = pos + (vec3(0.0) - pos) * speed;
    }
};