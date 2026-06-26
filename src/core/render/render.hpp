#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../math/essentials.hpp"
#include "../files/model.hpp"

#include "shader.hpp"



struct Vertex {
    vec3 position;
    vec4 color;
    vec2 uv;
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
    GLuint vao;
    GLuint vbo;

    std::vector<Vertex> vertices;
    std::vector<Model> models;

    int MAX_VERTICES;

    Mesh checkModel(std::string name) {
        Mesh result;
        for (size_t i = 0; i < models.size(); i++) {
            Model m = models[i];
            if (m.name == name) {
                result = m.mesh;
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

public:
    Shader shader;

    vec4 clear;

    Render() {
        clear = vec4(vec3(0.0), 1.0);
        MAX_VERTICES = 300;
    }
    
    void init() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
        glEnableVertexAttribArray(2);

        shader.load();
    }

    void init(std::string vert, std::string frag) {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
        glEnableVertexAttribArray(2);

        shader.load(vert, frag);
    }

    void beginFrame() {
        vertices.clear();

        glClearColor(clear.x, clear.y, clear.z, clear.w);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void endFrame() {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

        shader.bind();

        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }

    void drawTri(vec2 a, vec2 b, vec2 c, vec4 color) {
        vertices.push_back({vec3(a, 0.0), color, vec2(0.0)});
        vertices.push_back({vec3(b, 0.0), color, vec2(0.0, 1.0)});
        vertices.push_back({vec3(c, 0.0), color, vec2(1.0)});
    }

    void drawTri(vec3 a, vec3 b, vec3 c, vec4 color) {
        vertices.push_back({a, color, vec2(0.0)});
        vertices.push_back({b, color, vec2(0.0, 1.0)});
        vertices.push_back({c, color, vec2(1.0)});
    }

    void drawQuad(vec2 a, vec2 b, vec2 c, vec2 d, vec4 color) {
        vertices.push_back({vec3(a, 0.0), color, vec2(0.0)});
        vertices.push_back({vec3(b, 0.0), color, vec2(0.0, 1.0)});
        vertices.push_back({vec3(c, 0.0), color, vec2(1.0)});

        vertices.push_back({vec3(a, 0.0), color, vec2(0.0)});
        vertices.push_back({vec3(d, 0.0), color, vec2(1.0, 0.0)});
        vertices.push_back({vec3(c, 0.0), color, vec2(1.0)});
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
        vertices.push_back({start, color, vec2(0.0)});
        vertices.push_back({end, color, vec2(1.0)});
    }

    void drawMesh(Mesh& mesh) {
        shader.bind();
        mesh.draw();
    }

    void drawMesh(int c) {
        shader.bind();
        models[c].mesh.draw();
    }

    void drawMesh(std::string c) {
        shader.bind();
        Mesh m = checkModel(c);
        m.draw();
    }

};