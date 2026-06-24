#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

#include "core/render/window.hpp"
#include "core/render/render.hpp"

#include "core/math/essentials.hpp"
#include "engine/mobs/player.hpp"

int main() {
    Window w = Window(1280, 720, "Red Noise");
    Render render;
    render.init();

    Player player;
    player.pos = vec2(0.0);
    player.vel = vec2(0.0);
    player.speed = 0.001;

    float aspect = 1280.0/720.0;
    int tick = 0;

    while (w.isOpen()) {
        w.pollEvents();
        render.beginFrame();

        tick += 1;

        if (tick % 5 == 0) {
            player.checkMovement(w);
        }

        render.drawRect(player.pos, vec2(0.05 / aspect, 0.05), vec4(vec3(0.0), 1.0));

        vec2 p = floor(w.mouse * 16.0) / 16.0;

        render.drawRect(w.mouse, vec2(0.0125 / aspect, 0.0125), vec4(vec3(0.0), 1.0));

        render.drawRect(vec2(0, .8), vec2(.006 * player.stamina, aspect * .0125), vec4(135.0, 206.0, 235.0, 1.0));

        if (w.isKeyPressed("esc")) {
            break;
        }

        render.endFrame();
        w.swapBuffers();
    }

    w.close();

    return 0;
}