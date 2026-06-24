#pragma once

#include "../../core/render/window.hpp"

#include "../../core/math/essentials.hpp"


class Player {
    public:
        float speed;
        float stamina;
        vec2 pos;
        vec2 vel;
        Player(float spd, float stam) {
            speed = spd;
            stamina = stam;
            pos = vec2(0);
            vel = vec2(0);
        }
        void checkMovement(Window& w) {
            if (stamina > 0) {
                if (w.isKeyPressed("w")) {
                    if (w.isKeyPressed("shift")) {
                        vel.y += speed * .25;
                        stamina -= .1;
                    }
                    vel.y += speed;
                }
                if (w.isKeyPressed("s")) {
                    if (w.isKeyPressed("shift")) {
                        vel.y += -speed * .25;
                        stamina -= .1;
                    }
                    vel.y += -speed;
                }
                if (w.isKeyPressed("d")) {
                    if (w.isKeyPressed("shift")) {
                        vel.x += speed * .25;
                        stamina -= .1;
                    }
                    vel.x += speed;
                }
                if (w.isKeyPressed("a")) {
                    if (w.isKeyPressed("shift")) {
                        vel.x += -speed * .25;
                        stamina -= .1;
                    }
                    vel.x += -speed;
                }
            } else {

            }

            vel = vel * 0.9;
            pos = pos + vel;
        }
};

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
