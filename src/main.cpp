#include <iostream>

#include "core/render/window.hpp"

int main() {
    Window w = Window(1280, 720, "Red Noise");

    while (w.isOpen()) {
        std::cout << "WORKS!!!";

        if (w.isKeyPressed("esc")) {
            break;
        }
    }

    return 0;
}