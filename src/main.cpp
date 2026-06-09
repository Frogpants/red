#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

#include "core/render/window.hpp"
#include "core/render/render.hpp"

#include "core/math/essentials.hpp"

vec2 player = vec2(0.0);
vec2 vel = vec2(0.0);

int main() {
    Window w = Window(1280, 720, "Red Noise");
    Render render;
    render.init();

    float aspect = 1280.0/720.0;
    float speed = 0.001;

    int tick = 0;

    while (w.isOpen()) {
        w.pollEvents();
        render.beginFrame();

        tick += 1;

        if (tick % 5 == 0) {
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
            player = player + vel;
        }

        
        render.drawRect(player, vec2(0.05 / aspect, 0.05), vec4(vec3(0.0), 1.0));

        vec2 p = floor(w.mouse * 16.0) / 16.0;

        render.drawRect(w.mouse, vec2(0.0125 / aspect, 0.0125), vec4(vec3(0.0), 1.0));

        if (w.isKeyPressed("esc")) {
            break;
        }

        render.endFrame();
        w.swapBuffers();
    }

    w.close();

    return 0;
}