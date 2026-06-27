#pragma once

#include "../../core/math/essentials.hpp"
#include "../../core/render/render.hpp"
#include "../../core/render/camera.hpp"


struct Tile {
    vec3 pos;

    int index;
    float lightLevel = 0.0;

    vec2 tileC;
    vec2 tileS;

    void update(Render& r, Camera& cam, float WIDTH, std::vector<int>& world) {
        vec3 temp = pos - cam.pos;
        if (abs(temp.x) > tileC.x * (tileS.x / 2.0)) {
            if (temp.x < 0.0) {
                pos.x += tileC.x * tileS.x;
                index += tileC.x;
            } else {
                pos.x -= tileC.x * tileS.x;
                index -= tileC.x;
            }
        }

        if (abs(temp.z) > tileC.y * (tileS.y / 2.0)) {
            if (temp.z < 0.0) {
                pos.z += tileC.y * tileS.y;
                index += tileC.y * WIDTH;
            } else {
                pos.z -= tileC.y * tileS.y;
                index -= tileC.y * WIDTH;
            }
        }

        // r.drawMesh(world[index]);

        if (world[index] == 1) {
            vec3 a = pos + vec3(-tileS.x, 0.0, -tileS.y) * 0.5;
            vec3 b = pos + vec3(-tileS.x, 0.0, tileS.y) * 0.5;
            vec3 c = pos + vec3(tileS.x, 0.0, tileS.y) * 0.5;
            vec3 d = pos + vec3(tileS.x, 0.0, -tileS.y) * 0.5;
            r.drawQuad(a, b, c, d, vec4(0.416, 0.745, 0.188, 1.0));
        } else {
            r.drawLine(pos, pos + vec3(0.0, tileS.x / 1.5, 0.0), vec4(0.5, 0.5, 1.0, 1.0));
        }
    }
};