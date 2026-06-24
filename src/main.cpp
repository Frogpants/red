#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

#include "core/render/window.hpp"
#include "core/render/render.hpp"
#include "core/render/camera.hpp"

#include "core/math/essentials.hpp"

#include "engine/mobs/player.hpp"

int main() {
    Window w = Window(1280, 720, "Red Noise");
    Render render;
    render.init();

    Player player;
    player.pos = vec3(0.0, -0.05, 0.0);
    player.vel = vec3(0.0);
    player.speed = 0.001;

    Camera camera;
    camera.pos = vec3(0.0, 0.0, 0.0);

    float av = 0.0;

    float aspect = 1280.0/720.0;
    int tick = 0;

    while (w.isOpen()) {
        w.pollEvents();
        render.beginFrame();

        render.shader.setUniform((GLchar*)"cam", camera.pos);
        render.shader.setUniform((GLchar*)"rot", camera.rot);
        render.shader.setUniform((GLchar*)"f", camera.focal);

        tick += 1;

        if (tick % 5 == 0) {
            player.checkMovement(w, camera);

            if (w.isKeyPressed("e")) {
                av += 0.1;
            }
            if (w.isKeyPressed("q")) {
                av -= 0.1;
            }

            if (w.isKeyPressed("x")) {
                camera.FOV -= 1;
            }
            if (w.isKeyPressed("z")) {
                camera.FOV += 1;
            }
            camera.focal = 1.0 / atan(radians(camera.FOV / 2.0));

            av *= 0.9;
            camera.rot.x = camera.rot.x + av;
        }

        camera.follow(player.pos, 0.01);

        
        render.drawRect(vec2(0, -0.8), vec2(.006 * player.stamina, aspect * .0125), vec4(135.0, 206.0, 235.0, 1.0));

        render.drawQuad(vec3(-0.5, 0.0, -0.5), vec3(-0.5, 0.0, 0.5), vec3(0.5, 0.0, 0.5), vec3(0.5, 0.0, -0.5), vec4(1.0));
        render.drawRect(player.pos, vec2(0.05 / aspect, 0.05), vec4(1.0, 0.0, 0.0, 1.0));

        if (w.isKeyPressed("esc")) {
            break;
        }

        render.endFrame();
        w.swapBuffers();
    }

    w.close();

    return 0;
}