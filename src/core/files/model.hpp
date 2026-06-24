#pragma once

#include <fstream>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../math/essentials.hpp"

struct Vert {
    vec3 position;
    vec4 color;
    vec2 uv;
    vec3 normal;
};

class Mesh {
public:
    GLuint vao;
    GLuint vbo;

    int vertexCount;

    Mesh() {
        vao = 0;
        vbo = 0;
        vertexCount = 0;
    }

    void upload(std::vector<Vert>& vertices) {
        vertexCount = vertices.size();

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vert), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), (void*)offsetof(Vert, position));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vert), (void*)offsetof(Vert, color));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vert), (void*)offsetof(Vert, uv));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), (void*)offsetof(Vert, normal));
        glEnableVertexAttribArray(3);

        glBindVertexArray(0);
    }

    void draw() {
        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }
};


inline Mesh LoadOBJ(const std::string& path)
{
    std::ifstream file(path);

    std::vector<vec3> positions;
    std::vector<vec2> uvs;
    std::vector<vec3> normals;

    std::vector<Vert> vertices;

    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);

        std::string type;
        ss >> type;

        if (type == "v") {
            float x, y, z;
            ss >> x >> y >> z;

            positions.push_back(vec3(x, y, z));
        } else if (type == "vt") {
            float u, v;
            ss >> u >> v;

            uvs.push_back(vec2(u, v));
        } else if (type == "vn") {
            float x, y, z;
            ss >> x >> y >> z;

            normals.push_back(vec3(x, y, z));
        } else if (type == "f") {
            for (int i = 0; i < 3; i++) {
                std::string vert;
                ss >> vert;

                std::stringstream vs(vert);

                std::string pStr;
                std::string uvStr;
                std::string nStr;

                std::getline(vs, pStr, '/');
                std::getline(vs, uvStr, '/');
                std::getline(vs, nStr, '/');

                int p = std::stoi(pStr) - 1;
                int uv = std::stoi(uvStr) - 1;
                int n = std::stoi(nStr) - 1;

                Vert v;
                v.position = positions[p];
                v.uv = uvs[uv];
                v.normal = normals[n];
                v.color = vec4(1.0);

                vertices.push_back(v);
            }
        }
    }

    Mesh mesh;
    mesh.upload(vertices);

    return mesh;
}