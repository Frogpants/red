#pragma once


#include "../../core/math/essentials.hpp"
#include "../../core/render/window.hpp"


struct Player {
    float speed;
    vec2 pos;
    vec2 vel;

    float stamina = 100.0;

    void checkMovement(Window& w) {
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
            vel.y += speed * mult;
        }
        if (w.isKeyPressed("s")) {
            vel.y += -speed * mult;
        }
        if (w.isKeyPressed("d")) {
            vel.x += speed * mult;
        }
        if (w.isKeyPressed("a")) {
            vel.x += -speed * mult;
        }

        vel = vel * 0.9;
        pos = pos + vel;
    }
};

