#pragma once


#include "../../core/math/essentials.hpp"
#include "../../core/render/window.hpp"


struct Player {
    float speed;
    vec2 playerPos;
    vec2 vel;

    void checkMovement(Window& w) {
        if (w.isKeyPressed("w")) {
            vel.y += speed;
        }
        if (w.isKeyPressed("s")) {
            vel.y += -speed;
        }
        if (w.isKeyPressed("d")) {
            vel.x += speed;
        }
        if (w.isKeyPressed("a")) {
            vel.x += -speed;
        }

        vel = vel * 0.9;
        playerPos = playerPos + vel;
    }
};
