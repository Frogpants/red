#pragma once

#include "../../core/math/essentials.hpp"
#include "../../core/render/render.hpp"
#include "../../core/render/camera.hpp"

inline float tileXC = 0.0;
inline float tileYC = 0.0;

inline float tileXS = 0.0;
inline float tileYS = 0.0;

struct Tile {
    vec3 pos;

    int index;
    float lightLevel = 0.0;

    void update(Render& r, Camera cam, float WIDTH, std::vector<int> world) {
        vec3 temp = pos - cam.pos;
        if (abs(temp.x) > tileXC * (tileXS / 2.0)) {
            if (temp.x < 0.0) {
                pos.x += tileXC * tileXS;
                index += tileXC;
            } else {
                pos.x -= tileXC * tileXS;
                index -= tileXC;
            }
        }

        if (abs(temp.y) > tileYC * (tileYS / 2.0)) {
            if (temp.y < 0.0) {
                pos.y += tileYC * tileYS;
                index += tileXC * WIDTH;
            } else {
                pos.y -= tileXC * tileYS;
                index -= tileXC * WIDTH;
            }
        }

        r.drawMesh(world[index]);
    }
};