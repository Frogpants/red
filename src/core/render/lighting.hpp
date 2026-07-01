#pragma once

#include <vector>

#include <GL/glew.h>

#include "render.hpp"
#include "camera.hpp"

struct PointLight {
    vec3 position;
    float radius;
    vec3 color;
    float intensity;
};

class Lighting {
public:
    std::vector<PointLight> lights;

    RenderTexture shadow;
    GLuint lightBuffer = 0;

    vec3 worldLightDir = normalize(vec3(-0.45f, -1.0f, -0.25f));
    vec3 worldLightColor = vec3(1.0f, 0.97f, 0.92f);
    float worldLightIntensity = 0.55f;

    Lighting() = default;

    void init(int shadowWidth, int shadowHeight) {
        shadow.create(shadowWidth, shadowHeight, "shadow/vert.glsl", "shadow/frag.glsl", "post/vert.glsl", "post/frag.glsl", GL_R32F, GL_RED, GL_FLOAT);

        glGenTextures(1, &lightBuffer);
        glBindTexture(GL_TEXTURE_2D, lightBuffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    void addLight(const PointLight& light) {
        lights.push_back(light);
    }

    void setupDemoLights(const vec3& cameraPos, const vec3& carPos) {
        lights.clear();
        // lights.push_back({cameraPos + vec3(-1.2f, 1.8f, -0.8f), 8.0f, vec3(1.0f, 0.94f, 0.82f), 1.25f});
        lights.push_back({vec3(0.0, 1.0, 0.0), 14.0f, vec3(0.45f, 0.65f, 1.0f), 0.85f});
        // lights.push_back({carPos + vec3(0.0f, 0.8f, 0.0f), 4.5f, vec3(1.0f, 0.42f, 0.18f), 0.95f});
    }

    void updateDemoLightPositions(const vec3& cameraPos, const vec3& carPos) {
        if (lights.size() < 3) {
            setupDemoLights(cameraPos, carPos);
            return;
        }

        lights[0].position = cameraPos + vec3(-1.2f, 1.8f, -0.8f);
        lights[2].position = carPos + vec3(0.0f, 0.8f, 0.0f);
    }

    void setWorldLight(vec3 direction, vec3 color, float intensity) {
        worldLightDir = normalize(direction);
        worldLightColor = color;
        worldLightIntensity = intensity;
    }

    void updateLightBuffer(Shader& shader) {
        std::vector<float> lightData;
        lightData.reserve(lights.size() * 8);

        for (const PointLight& light : lights) {
            lightData.push_back(light.position.x);
            lightData.push_back(light.position.y);
            lightData.push_back(light.position.z);
            lightData.push_back(light.radius);

            lightData.push_back(light.color.x);
            lightData.push_back(light.color.y);
            lightData.push_back(light.color.z);
            lightData.push_back(light.intensity);
        }

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, lightBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (GLsizei)(lights.size() * 2), 1, 0, GL_RGBA, GL_FLOAT, lightData.data());
        shader.setUniform("lightTex", 2);
        shader.setUniform("lightCount", (int)lights.size());
    }

    void updateShadowBasis(vec3& shadowRight, vec3& shadowUp, vec3& shadowForward) const {
        shadowForward = normalize(vec3(-worldLightDir.x, -worldLightDir.y, -worldLightDir.z));
        vec3 worldUp = fabs(shadowForward.y) > 0.95f ? vec3(0.0f, 0.0f, 1.0f) : vec3(0.0f, 1.0f, 0.0f);
        shadowRight = normalize(cross(worldUp, shadowForward));
        shadowUp = normalize(cross(shadowForward, shadowRight));
    }

    void bindSceneUniforms(Shader& shader, const Camera& camera, const vec3& shadowRight, const vec3& shadowUp, const vec3& shadowForward, float shadowRange, float shadowBias) {
        shader.setUniform("cameraPos", camera.pos);
        shader.setUniform("worldLightDir", worldLightDir);
        shader.setUniform("worldLightColor", worldLightColor);
        shader.setUniform("worldLightIntensity", worldLightIntensity);
        shader.setUniform("shadowOrigin", camera.pos);
        shader.setUniform("shadowRight", shadowRight);
        shader.setUniform("shadowUp", shadowUp);
        shader.setUniform("shadowForward", shadowForward);
        shader.setUniform("shadowRange", shadowRange);
        shader.setUniform("shadowBias", shadowBias);
    }

    void bindShadowUniforms(Shader& shader, const Camera& camera, const vec3& shadowRight, const vec3& shadowUp, const vec3& shadowForward, float shadowRange) {
        shader.setUniform("shadowOrigin", camera.pos);
        shader.setUniform("shadowRight", shadowRight);
        shader.setUniform("shadowUp", shadowUp);
        shader.setUniform("shadowForward", shadowForward);
        shader.setUniform("shadowRange", shadowRange);
    }

    void bindTextures(Shader& shader) {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadow.texture);
        shader.setUniform("shadowTex", 3);
    }
};