#pragma once

#include "essentials.hpp"

class Noise {
    float seed;

    vec2 random2(vec2 st){
        st = vec2( dot(st,vec2(127.1,311.7)), dot(st,vec2(269.5,183.3)) );
        return fract(sin(st)*43758.5453123)*2.0 - 1.0;
    }

    float noise(vec2 st) {
        vec2 i = floor(st);
        vec2 f = fract(st);

        vec2 u = f*f*(f*-2.0 + 3.0);

        return mix( mix( dot( random2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ),
                        dot( random2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), u.x),
                    mix( dot( random2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ),
                        dot( random2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), u.x), u.y);
    }
};