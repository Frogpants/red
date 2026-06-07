#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../math/essentials.hpp"



struct Vertex {
    vec3 pos;
    vec4 color;
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

public:
    vec4 clear;

    Render() {
        clear = vec4(vec3(0.0), 1.0);
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

    void drawTri(vec3 a, vec3 b, vec3 c, vec4 color) {
        vertices.push_back({a, color});
        vertices.push_back({b, color});
        vertices.push_back({c, color});
    }

    void drawLine(vec3 start, vec3 end, vec4 color) {
        vertices.push_back({start, color});
        vertices.push_back({end, color});
    }

};