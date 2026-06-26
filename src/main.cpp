#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

#include "core/render/window.hpp"
#include "core/render/render.hpp"
#include "core/render/camera.hpp"

#include "core/math/essentials.hpp"

#include "engine/mobs/player.hpp"

#include "engine/world/world.hpp"

int main() {
    Window w = Window(1280, 720, "Red Noise");
    Render render;
    render.init();


    Player player;
    player.pos = vec3(0.0, -0.05, 0.0);
    player.vel = vec3(0.0);
    player.speed = 0.001;

    Camera camera;
    camera.pos = vec3(0.0);
    //camera.rot = vec3(0.0);

    vec2 start = vec2(0.0);

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
                camera.FOV -= 2;
            }
            if (w.isKeyPressed("z")) {
                camera.FOV += 2;
            }
            camera.focal = 1.0 / atan(radians(camera.FOV / 2.0));


            if (w.mouseDown(2)) {
                vec2 delta = w.mouse - start;
                camera.rot.x = camera.rot.x + delta.x * 1280.0 * 0.1;
                start = w.mouse;
            } else {
                start = w.mouse;

                if (w.mouseDown()) {

                }
            }

            av *= 0.9;
            camera.rot.x = camera.rot.x + av;

        }

        camera.follow(player.pos, 0.01);

        
        render.drawRect(vec2(0, -0.8), vec2(.006 * player.stamina, aspect * .0125), vec4(135.0/255.0, 206.0/255.0, 235.0/255.0, 1.0));
        render.drawRect(player.pos, vec2(0.05 / aspect, 0.05), vec4(1.0, 0.0, 0.0, 1.0));
        render.drawQuad(vec3(-0.5, 0.0, -0.5), vec3(-0.5, 0.0, 0.5), vec3(0.5, 0.0, 0.5), vec3(0.5, 0.0, -0.5), vec4(0.416, 0.745, 0.188, 1.0));
        

        if (w.isKeyPressed("esc")) {
            break;
        }

        render.endFrame();
        w.swapBuffers();
    }

    w.close();

    return 0;
}