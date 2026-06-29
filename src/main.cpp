#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

#include "core/render/window.hpp"
#include "core/render/render.hpp"
#include "core/render/rendertexture.hpp"
#include "core/render/camera.hpp"

#include "core/math/essentials.hpp"
#include "core/math/ray.hpp"

#include "engine/mobs/player.hpp"
#include "engine/mobs/vehicle.hpp"

#include "engine/world/world.hpp"



int main() {
    Window w = Window(1280, 720, "Red Noise");
    Render render;
    render.init();

    render.addModel("cube", "src/assets/models/objs/cube.obj");
    render.addModel("monkey", "src/assets/models/objs/monkey.obj");
    render.addModel("moon", "src/assets/models/objs/Moon-2K.obj");
    render.addModel("car", "src/assets/models/objs/Dodge_Charger_Low.obj");


    Player player;
    player.pos = vec3(0.0, 0.11, 0.0);
    player.vel = vec3(0.0);
    player.speed = 0.001;

    Vehicle car;
    car.pos = vec3(0.2, -0.02, 0.0);
    car.vel = vec3(0.0);
    car.accel = vec3(0.0);
    car.speed = 0.001;


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
    scene.triShader.addTex("src/assets/textures/Diffuse_2K.png");
    Texture moon = scene.triShader.textures[1];

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
        glBindTexture(GL_TEXTURE_2D, moon.texture);
        scene.triShader.setUniform("tex", 1);

        scene.triShader.setUniform("lightDir", vec3(0.5f, 1.0f, 0.5f));

        scene.triShader.setUniform("lightColor", vec3(1.0f));
        scene.triShader.setUniform("ambient", vec3(0.0));

        tick += 1;

        if (tick % 2 == 0) {
            if (player.inVehicle) {
                car.checkMovement(w, camera);
            } else {
                player.checkMovement(w, camera);
            }

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
                camera.rot = camera.rot - vec3(delta, 0.0) * 1280.0 * 0.1;
                start = w.mouse;
            } else {
                start = w.mouse;

                if (w.mouseDown()) {

                }
            }

            av *= 0.9;
            camera.rot.x = camera.rot.x + av;

        }

        if (w.isKeyPressed("1")) {
            player.inVehicle = true;
        }
        if (w.isKeyPressed("2")) {
            player.inVehicle = false;
        }

        player.vel = player.vel * 0.9;
        player.pos = player.pos + player.vel;

        car.vel = car.vel + car.accel * 0.1;
        car.vel = car.vel * 0.9;
        car.pos = car.pos + car.vel;
        car.rv = car.rv * 0.9;
        car.dir = car.dir + car.rv.x;
        car.rot.x = car.dir + 135.0;

        

        player.rot.x = (-180.0 * atan2((w.mouse.y + camera.pos.z) - player.pos.z, (w.mouse.x + camera.pos.x) - player.pos.x)) / 3.14159;

        if (player.inVehicle) {
            camera.follow(car.pos, 0.05);
        } else {
            render.drawMesh(scene, "cube", player.pos, player.rot, vec3(0.004, 0.01777, 0.004) * 5.0, vec4(1.0, 0.5, 0.5, 1.0));
            camera.follow(player.pos, 0.05);
        }

        world.render(scene, render, camera, w.mouse);

        
        render.drawRect(player.pos + vec3(0.0, 0.8, 0.0), vec2(.006 * player.stamina, aspect * .0125), vec4(135.0/255.0, 206.0/255.0, 235.0/255.0, 1.0));
        // render.drawMesh(scene, "cube", player.pos + vec3(0.0, 0.15, 0.0), camera.rot * vec3(-1.0, 0.0, 1.0), vec3(0.001 * player.stamina, 0.01, aspect * 0.001), vec4(135.0/255.0, 206.0/255.0, 235.0/255.0, 1.0));
        // render.drawMesh(scene, "moon", vec3(0.0, 0.2, 0.0), vec3((float)tick * 0.1, 0.0, 0.0), vec3(0.1) / vec3(aspect, 1.0, aspect), vec4(1.0));
        // render.drawMesh(scene, "car", car.pos, car.rot, vec3(0.1) / vec3(aspect, 1.0, aspect), vec4(1.0));
        
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