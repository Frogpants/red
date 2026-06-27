#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

#include "core/render/window.hpp"
#include "core/render/render.hpp"
#include "core/render/rendertexture.hpp"
#include "core/render/camera.hpp"

#include "core/math/essentials.hpp"
#include "core/math/ray.hpp"

#include "engine/mobs/player.hpp"

#include "engine/world/world.hpp"



int main() {
    Window w = Window(1280, 720, "Red Noise");
    Render render;
    render.init();


    Player player;
    player.pos = vec3(0.0, 0.05, 0.0);
    player.vel = vec3(0.0);
    player.speed = 0.001;


    Camera camera;
    camera.pos = vec3(0.0);


    World world;
    world.tileC = vec2(10.0);
    world.tileS = vec2(0.1);
    world.create(100.0, 100.0);


    RenderTexture scene;
    scene.create(1280, 720, "game/vert.glsl", "game/frag.glsl", "post/vert.glsl", "post/frag.glsl");

    scene.triShader.addTex("src/assets/textures/null.png");
    Texture missing = scene.triShader.textures[0];

    RenderTexture post;
    post.create(1280, 720, "game/vert.glsl", "game/frag.glsl", "post/vert.glsl", "post/frag.glsl");


    vec2 start = vec2(0.0);


    float av = 0.0;

    float aspect = 1280.0/720.0;
    int tick = 0;

    float t;
    Ray mRay = mouseRay(w.mouse, camera);

    while (w.isOpen()) {
        w.pollEvents();
        render.beginFrame(scene);

        scene.triShader.bind();
        scene.triShader.setUniform("cam", camera.pos);
        scene.triShader.setUniform("rot", camera.rot);
        scene.triShader.setUniform("f", camera.focal);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, missing.texture);
        scene.triShader.setUniform("tex", 1);

        scene.triShader.setUniform("lightDir", normalize(camera.rot));
        scene.triShader.setUniform("lightColor", vec3(1.0));
        scene.triShader.setUniform("ambient", 1.0);

        tick += 1;

        if (tick % 4 == 0) {
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
                camera.rot = camera.rot + vec3(delta, 0.0) * 1280.0 * 0.1;
                start = w.mouse;
            } else {
                start = w.mouse;

                if (w.mouseDown()) {

                }
            }

            av *= 0.9;
            camera.rot.x = camera.rot.x + av;

        }

        player.vel = player.vel * 0.9;
        player.pos = player.pos + player.vel;

        camera.follow(player.pos, 0.05);


        world.render(render, camera);

        
        // render.drawRect(vec2(0, -0.8), vec2(.006 * player.stamina, aspect * .0125), vec4(135.0/255.0, 206.0/255.0, 235.0/255.0, 1.0));
        render.drawRect(player.pos, vec2(0.05 / aspect, 0.05), vec4(1.0, 0.0, 0.0, 1.0));

        if (w.isKeyPressed("r")) {
            camera.rot = vec3(0.0);
        }


        if (w.isKeyPressed("esc")) {
            break;
        }

        render.endFrame();


        render.beginFrame(post);
        render.drawTexture(scene);
        render.endFrame();


        render.present(post, 1280, 720);


        w.swapBuffers();
    }

    w.close();

    return 0;
}