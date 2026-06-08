#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../math/essentials.hpp"
#include "shader.hpp"



struct Vertex {
    vec2 pos;
    vec4 color;
    vec2 uv;
};

struct Tri {
    Vertex v1;
    Vertex v2;
    Vertex v3;
};



class Render {
private:
    GLuint vao;
    GLuint vbo;

    Shader shader;

    std::vector<Vertex> vertices;

    int MAX_VERTICES;

public:
    vec4 clear;

    Render() {
        clear = vec4(vec3(0.0), 1.0);
        MAX_VERTICES = 3;
    }
    
    void init() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
        glEnableVertexAttribArray(2);

        shader.load();
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
        vertices.push_back({a, color, vec2(0.0)});
        vertices.push_back({b, color, vec2(0.0, 1.0)});
        vertices.push_back({c, color, vec2(1.0)});
    }

    void drawLine(vec2 start, vec2 end, vec4 color) {
        vertices.push_back({start, color, vec2(0.0)});
        vertices.push_back({end, color, vec2(1.0)});
    }

};