#pragma once

#include <vector>

#include "essentials.hpp"

inline vec3 cam = vec3(0.0);
inline vec3 rot = vec3(0.0);

inline float FOV = 90.0;
inline float f = 240.0;

inline std::vector<vec3> getTrigVals() {
    std::vector<vec3> result;
    result.push_back(sin(rot));
    result.push_back(cos(rot));
    return result;
}

inline vec2 project(vec3 pos) {
    std::vector<vec3> vals = getTrigVals();
    vec3 p = pos - cam;

    vec2 t1 = vec2(p.x, p.z);
    vec2 t2 = vec2(vals[1].x, -vals[0].x);
    p.x = dot(t1, t2);
    t2 = vec2(vals[0].x, vals[1].x);
    p.z = dot(t1, t2);



    return vec2(p.x/p.z, p.y/p.z) * f;

}