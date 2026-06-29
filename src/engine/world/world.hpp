#pragma once

#include <vector>

#include "../../core/math/essentials.hpp"
#include "../../core/math/noise.hpp"

#include "../../core/render/render.hpp"
#include "../../core/render/rendertexture.hpp"
#include "../../core/render/camera.hpp"

#include "tile.hpp"


struct World {
    vec2 size;

    vec2 tileC;
    vec2 tileS;

    std::vector<int> world;
    std::vector<float> lighting;

    std::vector<Tile> tiles;

    void create(float width, float height) {
        Noise noise;

        size = vec2(width, height);
        for (int i = 0; i < width * height; i++) {
            int type = noise.randInt(1, 4);
            world.push_back(type);
            lighting.push_back(0.0);
        }

        int index = 0;
        vec2 p = vec2(0.0, tileS.y/2.0);
        for (int y = 0; y < (int)tileC.y; y++) {
            p.x = tileS.x/2.0;
            for (int x = 0; x < (int)tileC.x; x++) {
                addTile(vec3(p.x, 0.0, p.y), index);
                p.x += tileS.x;
                index += 1;
            }
            p.y += tileS.y;
            index += width - tileC.x;
        }
    }

    void addTile(vec3 pos, int index) {
        Tile tile;
        tile.pos = pos;
        tile.index = index;
        tile.lightLevel = 0.0;
        tiles.push_back(tile);
    }

    void render(RenderTexture& s, Render& r, Camera& c, vec2 m) {
        for (Tile& tile : tiles) {
            tile.tileC = tileC;
            tile.tileS = tileS;
            tile.update(s, r, c, size.x, world, m);
        }
    }

};