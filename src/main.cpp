#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION

#include "core/render/window.hpp"
#include "core/render/render.hpp"
#include "core/render/lighting.hpp"
#include "core/render/rendertexture.hpp"
#include "core/render/camera.hpp"

#include "core/math/essentials.hpp"
#include "core/math/ray.hpp"

#include "engine/mobs/player.hpp"
#include "engine/mobs/vehicle.hpp"

#include "engine/world/world.hpp"



vec3 screenToWorld(vec2 mouse, float depth, Camera& c) {
    vec3 p(mouse.x / c.focal, mouse.y / c.focal, depth);

    vec3 r = radians(c.rot);

    float x = p.x;
    float y = p.y;
    p.x = x * cos(-r.z) - y * sin(-r.z);
    p.y = x * sin(-r.z) + y * cos(-r.z);

    y = p.y;
    float z = p.z;
    p.y = y * cos(-r.y) - z * sin(-r.y);
    p.z = y * sin(-r.y) + z * cos(-r.y);

    x = p.x;
    z = p.z;
    p.x = x * cos(-r.x) - z * sin(-r.x);
    p.z = x * sin(-r.x) + z * cos(-r.x);

    p += c.pos;

    return p;
}


int main() {
    Window w = Window(1280, 720, "Red Noise");
    Render render;
    render.init();

    render.addModel("cube", "src/assets/models/objs/cube.obj");
    render.addModel("monkey", "src/assets/models/objs/monkey.obj");
    render.addModel("moon", "src/assets/models/objs/Moon-2K.obj");
    render.addModel("car", "src/assets/models/objs/Dodge_Charger_Low.obj");

    float aspect = 1280.0f / 720.0f;
    vec3 playerScale = vec3(0.004f, 0.01777f, 0.004f) * 5.0f;
    vec3 carScale = vec3(0.1f) / vec3(aspect, 1.0f, aspect);


    Player player;
    player.pos = vec3(0.0, 0.15, 0.0);
    player.vel = vec3(0.0);
    player.speed = 0.001;

    Vehicle car;
    car.pos = vec3(0.2, -0.02, 0.0);
    car.vel = vec3(0.0);
    car.accel = vec3(0.0);
    car.speed = 0.001;


    Camera camera;
    camera.pos = vec3(0.0);

    Lighting lighting;
    lighting.init(1024, 1024);


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

    RenderTexture ui;
    ui.create(1280, 720, "ui/vert.glsl", "ui/frag.glsl", "post/vert.glsl", "post/frag.glsl");

    lighting.setupDemoLights(camera.pos, car.pos);


    vec2 start = vec2(0.0);


    float av = 0.0;

    int tick = 0;

    bool complexLightingEnabled = true;
    bool lightingToggleLatch = false;

    float t;
    Ray mRay = mouseRay(w.mouse, camera);

    while (w.isOpen()) {
        w.pollEvents();

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

        bool lightingTogglePressed = w.isKeyPressed("l");
        if (lightingTogglePressed && !lightingToggleLatch) {
            complexLightingEnabled = !complexLightingEnabled;
            lightingToggleLatch = true;
            std::cout << "Complex lighting: " << (complexLightingEnabled ? "ON" : "OFF") << std::endl;
        } else if (!lightingTogglePressed) {
            lightingToggleLatch = false;
        }

        player.vel = player.vel * 0.9f;
        player.pos = player.pos + player.vel;

        car.vel = car.vel + car.accel * 0.1f;
        car.vel = car.vel * 0.9f;
        car.pos = car.pos + car.vel;
        car.accel = vec3(0.0f);

        car.rv = car.rv * 0.9;
        car.dir = car.dir + car.rv.x;
        car.rot.x = car.dir + 135.0;

        if (player.inVehicle) {
            player.pos = car.pos + vec3(0.0f, 0.15f, 0.0f);
        }

        player.rot.x = (-180.0 * atan2((w.mouse.y + camera.pos.z) - player.pos.z, (w.mouse.x + camera.pos.x) - player.pos.x)) / 3.14159;

        if (player.inVehicle) {
            camera.follow(car.pos, 0.05);
        } else {
            camera.follow(player.pos, 0.05);
        }

        if (complexLightingEnabled) {
            lighting.updateDemoLightPositions(camera.pos, car.pos);
        }

        vec3 shadowRight = vec3(1.0f, 0.0f, 0.0f);
        vec3 shadowUp = vec3(0.0f, 1.0f, 0.0f);
        vec3 shadowForward = vec3(0.0f, 0.0f, 1.0f);
        if (complexLightingEnabled) {
            lighting.updateShadowBasis(shadowRight, shadowUp, shadowForward);

            lighting.bindShadowUniforms(lighting.shadow.triShader, camera, shadowRight, shadowUp, shadowForward, 18.0f);

            render.beginFrame(lighting.shadow);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, moon.texture);
            lighting.shadow.triShader.setUniform("modelColor", vec4(1.0f));

            if (!player.inVehicle) {
                render.drawMesh(lighting.shadow, "cube", player.pos, player.rot, playerScale, vec4(1.0, 0.5, 0.5, 1.0));
            }
            world.render(lighting.shadow, render, camera, w.mouse);
            render.drawMesh(lighting.shadow, "car", car.pos, car.rot, carScale, vec4(1.0));
            render.endFrame();
        }

        render.beginFrame(scene);

        scene.triShader.bind();
        scene.triShader.setUniform("cam", camera.pos);
        scene.triShader.setUniform("rot", camera.rot);
        scene.triShader.setUniform("f", camera.focal);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, moon.texture);
        scene.triShader.setUniform("tex", 1);

        scene.triShader.setUniform("ambient", complexLightingEnabled ? vec3(0.15f) : vec3(0.55f));

        if (complexLightingEnabled) {
            lighting.bindSceneUniforms(scene.triShader, camera, shadowRight, shadowUp, shadowForward, 18.0f, 0.02f);
            lighting.bindTextures(scene.triShader);
            lighting.updateLightBuffer(scene.triShader);
        } else {
            scene.triShader.setUniform("cameraPos", camera.pos);
            scene.triShader.setUniform("worldLightDir", vec3(0.0f, -1.0f, 0.0f));
            scene.triShader.setUniform("worldLightColor", vec3(1.0f));
            scene.triShader.setUniform("worldLightIntensity", 0.0f);
            scene.triShader.setUniform("lightCount", 0);
        }

        if (!player.inVehicle) {
            render.drawMesh(scene, "cube", player.pos, player.rot, playerScale, vec4(1.0, 0.5, 0.5, 1.0));
        }

        world.render(scene, render, camera, w.mouse);

        // render.drawMesh(scene, "cube", player.pos + vec3(0.0, 0.15, 0.0), camera.rot * vec3(-1.0, 0.0, 1.0), vec3(0.001 * player.stamina, 0.01, aspect * 0.001), vec4(135.0/255.0, 206.0/255.0, 235.0/255.0, 1.0));
        // render.drawMesh(scene, "moon", vec3(0.0, 0.2, 0.0), vec3((float)tick * 0.1, 0.0, 0.0), vec3(0.1) / vec3(aspect, 1.0, aspect), vec4(1.0));
        render.drawMesh(scene, "car", car.pos, car.rot, vec3(0.1) / vec3(aspect, 1.0, aspect), vec4(1.0));
        
        if (w.isKeyPressed("r")) {
            camera.rot = vec3(45.0, 35.264, 0.0);
        }


        if (w.isKeyPressed("esc")) {
            break;
        }

        render.endFrame();


        // ui
        render.clear = vec4(0,0,0,0);
        render.beginFrame(ui, false);


        // render.drawRect(vec3(0.0), vec2(0.5), vec4(1.0));


        render.endFrame();
        render.clear = vec4(0,0,0,1);



        render.present(post, scene, &ui, 1280, 720);


        w.swapBuffers();
    }

    w.close();

    return 0;
}