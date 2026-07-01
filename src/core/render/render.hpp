#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../math/essentials.hpp"
#include "../files/model.hpp"

#include "shader.hpp"
#include "rendertexture.hpp"



struct Vertex {
    vec3 position;
    vec4 color;
    vec2 uv;
    vec3 normal;
};


struct Tri {
    Vertex v1;
    Vertex v2;
    Vertex v3;
};

struct Model {
    std::string name;
    Mesh mesh;
};


class Render {
private:
    RenderTexture* target;
    Shader* activeShader = nullptr;

    GLuint vao;
    GLuint vbo;

    GLuint quadVAO, quadVBO;

    std::vector<Vertex> vertices;
    

    int MAX_VERTICES;

    int checkModel(std::string name) {
        int result = -1;
        for (size_t i = 0; i < models.size(); i++) {
            Model m = models[i];
            if (m.name == name) {
                result = i;
                break;
            }
        }

        return result;
    }

    void cache(std::string path, std::string name) {
        Model m;
        m.mesh = LoadOBJ(path);
        m.name = name;
        models.push_back(m);
    }

    vec3 findNormal(vec3 a, vec3 b, vec3 c) {
        return normalize(cross(b - a, c - a));
    }

public:
    vec4 clear;
    std::vector<Model> models;

    Render() {
        clear = vec4(vec3(0.0), 1.0);
        MAX_VERTICES = 300000;
    }
    
    void init() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(3);


        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);

        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

        float quad[] = {
            // pos      // uv
            -1,-1, 0,   0,0,
            -1, 1, 0,   0,1,
            1, 1, 0,   1,1,

            -1,-1, 0,   0,0,
            1, 1, 0,   1,1,
            1,-1, 0,   1,0
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    void beginFrame(RenderTexture& rt, bool depth = true) {
        target=&rt;
        activeShader=&rt.triShader;

        if (depth) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, rt.fbo);
        glViewport(0, 0, rt.width, rt.height);

        vertices.clear();

        glClearColor(clear.x, clear.y, clear.z, clear.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void endFrame() {
        if (vertices.empty()) {
            return;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, target->fbo);
        glViewport(0, 0, target->width, target->height);

        activeShader->bind();

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        vertices.clear();
    }

    void drawTexture(RenderTexture& src)
    {
        activeShader = &target->postShader;

        activeShader->bind();

        activeShader->setTexture("sceneTex", src.texture, 0);
        activeShader->setTexture("uiTex", src.texture, 1);
        activeShader->setUniform("hasUi", 0);

        drawQuad(vec2(-1.0f, -1.0f), vec2(-1.0f,  1.0f), vec2( 1.0f,  1.0f), vec2( 1.0f, -1.0f), vec4(1.0f));
    }

    void present(RenderTexture& src, int width, int height) {
        present(src, src, nullptr, width, height);
    }

    void present(RenderTexture& src, RenderTexture& scene, int width, int height) {
        present(src, scene, nullptr, width, height);
    }

    void present(RenderTexture& src, RenderTexture& scene, RenderTexture* overlay, int width, int height) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, width, height);

        glDisable(GL_DEPTH_TEST);

        src.postShader.bind();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, scene.texture);
        src.postShader.setUniform("sceneTex", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, overlay != nullptr ? overlay->texture : scene.texture);
        src.postShader.setUniform("uiTex", 1);

        src.postShader.setUniform("hasUi", overlay != nullptr ? 1 : 0);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void drawTri(vec2 a, vec2 b, vec2 c, vec4 color) {
        vec3 v1 = vec3(a, 0.0);
        vec3 v2 = vec3(b, 0.0);
        vec3 v3 = vec3(c, 0.0);
        vec3 n = findNormal(v1, v2, v3);
        vertices.push_back({v1, color, vec2(0.0), n});
        vertices.push_back({v2, color, vec2(0.0, 1.0), n});
        vertices.push_back({v3, color, vec2(1.0), n});
    }

    void drawTri(vec3 a, vec3 b, vec3 c, vec4 color) {
        vec3 n = findNormal(a, b, c);
        vertices.push_back({a, color, vec2(0.0), n});
        vertices.push_back({b, color, vec2(0.0, 1.0), n});
        vertices.push_back({c, color, vec2(1.0), n});
    }

    void drawQuad(vec2 a, vec2 b, vec2 c, vec2 d, vec4 color) {
        vec3 v1 = vec3(a, 0.0);
        vec3 v2 = vec3(b, 0.0);
        vec3 v3 = vec3(c, 0.0);
        vec3 v4 = vec3(d, 0.0);
        vec3 n = findNormal(v1, v2, v3);
        vertices.push_back({v1, color, vec2(0.0), n});
        vertices.push_back({v2, color, vec2(0.0, 1.0), n});
        vertices.push_back({v3, color, vec2(1.0), n});

        vertices.push_back({v1, color, vec2(0.0), n});
        vertices.push_back({v4, color, vec2(1.0, 0.0), n});
        vertices.push_back({v3, color, vec2(1.0), n});
    }

    void drawQuad(vec3 a, vec3 b, vec3 c, vec3 d, vec4 color) {
        vertices.push_back({a, color, vec2(0.0)});
        vertices.push_back({b, color, vec2(0.0, 1.0)});
        vertices.push_back({c, color, vec2(1.0)});

        vertices.push_back({a, color, vec2(0.0)});
        vertices.push_back({d, color, vec2(1.0, 0.0)});
        vertices.push_back({c, color, vec2(1.0)});
    }

    void drawRect(vec2 pos, vec2 size, vec4 color) {
        vec2 a = pos - size;
        vec2 b = pos - vec2(size.x, -size.y);
        vec2 c = pos + size;
        vec2 d = pos + vec2(size.x, -size.y);
        drawQuad(a, b, c, d, color);
    }

    void drawRect(vec3 pos, vec2 size, vec4 color) {
        vec3 a = pos - vec3(size, 0.0);
        vec3 b = pos - vec3(size.x, -size.y, 0.0);
        vec3 c = pos + vec3(size, 0.0);
        vec3 d = pos + vec3(size.x, -size.y, 0.0);
        drawQuad(a, b, c, d, color);
    }

    void drawLine(vec2 start, vec2 end, vec4 color) {
        vertices.push_back({vec3(start, 0.0), color, vec2(0.0)});
        vertices.push_back({vec3(end, 0.0), color, vec2(1.0)});
    }

    void drawLine(vec3 start, vec3 end, vec4 color) {
        drawQuad(start + vec3(-0.001, 0.0, 0.0), start + vec3(0.001, 0.0, 0.0), end + vec3(0.001, 0.0, 0.0), end + vec3(-0.001, 0.0, 0.0), color);
    }

    void addModel(std::string name, std::string path) {
        int index = checkModel(name);
        if (index == -1) {
            cache(path, name);
        }
    }

    void drawMesh(Mesh& mesh, vec3 position = vec3(0.0f), vec3 rotation = vec3(0.0f), vec3 scale = vec3(1.0f), vec4 color = vec4(1.0f)) {
        activeShader->bind();

        activeShader->setUniform((GLchar*)"modelPos", position);
        activeShader->setUniform((GLchar*)"modelRot", rotation);
        activeShader->setUniform((GLchar*)"modelScale", scale);

        activeShader->setUniform((GLchar*)"modelColor", color);

        mesh.draw();
    }

    void drawMesh(int c, vec3 position = vec3(0.0f), vec3 rotation = vec3(0.0f), vec3 scale = vec3(1.0f), vec4 color = vec4(1.0f)) {
        activeShader->bind();

        activeShader->setUniform((GLchar*)"modelPos", position);
        activeShader->setUniform((GLchar*)"modelRot", rotation);
        activeShader->setUniform((GLchar*)"modelScale", scale);

        activeShader->setUniform((GLchar*)"modelColor", color);

        models[c].mesh.draw();
    }

    void drawMesh(RenderTexture& src, std::string c, vec3 position = vec3(0.0f), vec3 rotation = vec3(0.0f), vec3 scale = vec3(1.0f), vec4 color = vec4(1.0f)) {
        src.triShader.bind();
        int index = checkModel(c);
        Mesh m = models[index].mesh;

        src.triShader.setUniform((GLchar*)"modelPos", position);
        src.triShader.setUniform((GLchar*)"modelRot", rotation);
        src.triShader.setUniform((GLchar*)"modelScale", scale);

        src.triShader.setUniform((GLchar*)"modelColor", color);

        m.draw();
    }
};