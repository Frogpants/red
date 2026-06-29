#pragma once

#include "../../core/math/essentials.hpp"

#include "../../core/render/render.hpp"
#include "../../core/render/rendertexture.hpp"
#include "../../core/render/camera.hpp"


struct Tile {
    vec3 pos;

    int index;
    float lightLevel = 0.0;

    vec2 tileC;
    vec2 tileS;

    void update(RenderTexture& s, Render& r, Camera& cam, float WIDTH, std::vector<int>& world, vec2 m) {
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

        float height = 0.0;
        // if (index >= 0.0) {
        //     height = ((float)world[index]) * 0.002;
        // }

        r.drawMesh(s, "cube", pos + vec3(0.0, 2.0 * height, 0.0), vec3(0.0), vec3(0.01, 0.002 + height, 0.01) * 5.0);
    }
};