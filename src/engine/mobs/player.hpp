#pragma once


#include "../../core/math/essentials.hpp"
#include "../../core/render/window.hpp"
#include "../../core/render/camera.hpp"

struct Player {
    float speed;
    vec3 pos;
    vec3 vel;

    float dir = 0.0;

    float stamina = 100.0;

    void checkMovement(Window& w, Camera& c) {
        float mult = 1.0;
        if (w.isKeyPressed("shift") && stamina > 0.0) {
            mult = 1.25;
            stamina -= 0.5;
        } else {
            stamina += 0.1;
        }

        if (stamina < 0.0) {
            stamina = 0.0;
        } else if (stamina > 100.0) {
            stamina = 100.0;
        }

        if (w.isKeyPressed("w")) {
            move(c, mult, dir + 90.0);
        }
        if (w.isKeyPressed("s")) {
            move(c, -mult, dir + 90.0);
        }

        if (w.isKeyPressed("d")) {
            move(c, mult * 0.5, dir);
        }
        if (w.isKeyPressed("a")) {
            move(c, -mult * 0.5, dir);
        }

        vel = vel * 0.9;
        pos = pos + vel;
    }

    void move(Camera& c, float mult, float d) {
        float angle = d - c.rot.x;
        vel.x = vel.x - mult * speed * sin(radians(angle));
        vel.z = vel.z - mult * speed * cos(radians(angle));
    }
};

