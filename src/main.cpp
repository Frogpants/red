#include "core/render/window.hpp"
#include "core/render/render.hpp"

int main() {
    Window w = Window(1280, 720, "Red Noise");
    Render render;
    render.init();

    render.clear = vec4(vec3(0.5), 1.0);

    while (w.isOpen()) {
        w.pollEvents();
        render.beginFrame();

        render.drawTri(vec2(-0.5), vec2(0.5, -0.5), vec2(0.0, 0.5), vec4(vec3(0.0), 1.0));



        if (w.isKeyPressed("esc")) {
            break;
        }

        render.endFrame();
        w.swapBuffers();
    }

    w.close();

    return 0;
}