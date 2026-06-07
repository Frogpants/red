#include <iostream>

#include "core/render/window.hpp"

int main() {
    Window w = Window(1280, 720, "Red Noise");

    while (w.isOpen()) {
        w.pollEvents();

        if (w.isKeyPressed("esc")) {
            break;
        }
    }

    w.close();

    return 0;
}