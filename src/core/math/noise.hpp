#pragma once

#include <iostream>
#include <random>

#include "essentials.hpp"

struct Noise {
    inline static std::mt19937 gen{std::random_device{}()};
    
    float rand() {
        static std::uniform_real_distribution<float> dis(0.0f, 1.0f);
        return dis(gen);
    }

    int randInt(int a, int b) {
        std::uniform_int_distribution<int> dis(a, b);
        return dis(gen);
    }

    int randInt(vec2 p, int a, int b) {
        float n = noise(p);
        return a + (int)std::floor((b - a + 1) * n);
    }

    float random(vec2 p)
    {
        return fract(sin(dot(p, vec2(127.1f, 311.7f))) * 43758.5453f);
    }

    float noise (vec2 st) {
        vec2 i = floor(st);
        vec2 f = fract(st);

        // Four corners in 2D of a tile
        float a = random(i);
        float b = random(i + vec2(1.0, 0.0));
        float c = random(i + vec2(0.0, 1.0));
        float d = random(i + vec2(1.0, 1.0));

        // Smooth Interpolation

        // Cubic Hermine Curve.  Same as SmoothStep()
        vec2 u = f*f*(f * -2.0 + 3.0);
        // u = smoothstep(0.,1.,f);

        // Mix 4 coorners percentages
        return mix(a, b, u.x) + (c - a)* u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
    }
};