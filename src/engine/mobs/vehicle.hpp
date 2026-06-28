#pragma once


#include "../../core/math/essentials.hpp"

#include "../../core/render/window.hpp"
#include "../../core/render/camera.hpp"
#include "../../core/render/render.hpp"

struct Vehicle {
    float speed;

    vec3 pos;
    vec3 vel;
    vec3 accel;

    vec3 rv;
    vec3 rot;

    float dir = -135.0;

    float fuel = 100.0;

    void checkMovement(Window& w, Camera& c) {
        float mult = 1.0;
        if (w.isKeyPressed("shift") && fuel > 0.0) {
            mult = 1.25;
            fuel -= 0.01;
        } else {
            fuel += 0.1;
        }

        if (fuel < 0.0) {
            fuel = 0.0;
        } else if (fuel > 100.0) {
            fuel = 100.0;
        }

        accel = vec3(0.0);
        if (w.isKeyPressed("w")) {
            move(c, mult, dir);
        }
        if (w.isKeyPressed("s")) {
            move(c, -mult, dir);
        }

        if (w.isKeyPressed("d")) {
            rv.x += 0.1;
        }
        if (w.isKeyPressed("a")) {
            rv.x += -0.1;
        }
    }

    void move(Camera& c, float mult, float d) {
        float angle = c.rot.x - d;
        accel.x = mult * speed * sin(radians(angle));
        accel.z = mult * speed * cos(radians(angle));
    }
};

