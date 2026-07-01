#pragma once

#include <fstream>
#include <sstream>
#include <string>
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
    GLuint vao = 0;
    GLuint vbo = 0;

    GLsizei vertexCount = 0;
    vec3 localMin = vec3(0.0f);
    vec3 localMax = vec3(0.0f);
    bool hasBounds = false;

    Mesh() = default;

    void upload(const std::vector<Vert>& vertices) {
        vertexCount = (GLsizei)vertices.size();

        if (!vertices.empty()) {
            localMin = vertices[0].position;
            localMax = vertices[0].position;
            for (const Vert& v : vertices) {
                localMin = min(localMin, v.position);
                localMax = max(localMax, v.position);
            }
            hasBounds = true;
        }

        if (!vao) glGenVertexArrays(1, &vao);
        if (!vbo) glGenBuffers(1, &vbo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER,
                    vertexCount * sizeof(Vert),
                    vertices.data(),
                    GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vert), (void*)offsetof(Vert, color));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vert), (void*)offsetof(Vert, uv));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), (void*)offsetof(Vert, normal));
        glEnableVertexAttribArray(3);

        glBindVertexArray(0);

        // IMPORTANT: unbind buffer too
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void draw() const {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }
};

inline Mesh LoadOBJ(const std::string& path) {
    std::ifstream file(path);

    Mesh mesh;

    if (!file.is_open()) {
        std::cout << "Failed to open OBJ: " << path << std::endl;
        return mesh;
    }

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
            float x,y,z;
            ss >> x >> y >> z;

            positions.emplace_back(x,y,z);
        } else if (type == "vt") {
            float u,v;
            ss >> u >> v;

            uvs.emplace_back(u,v);
        } else if (type == "vn") {
            float x,y,z;
            ss >> x >> y >> z;

            normals.emplace_back(x,y,z);
        } else if (type == "f") {
            struct FaceIndex {
                int p=-1;
                int uv=-1;
                int n=-1;
            };

            std::vector<FaceIndex> face;

            std::string token;

            while (ss >> token) {
                FaceIndex idx;

                std::stringstream vs(token);

                std::string p,uv,n;

                std::getline(vs,p,'/');
                std::getline(vs,uv,'/');
                std::getline(vs,n,'/');

                if (!p.empty()) {
                    idx.p = std::stoi(p)-1;
                }

                if (!uv.empty()) {
                    idx.uv = std::stoi(uv)-1;
                }

                if (!n.empty()) {
                    idx.n = std::stoi(n)-1;
                }

                face.push_back(idx);
            }

            if (face.size() < 3) {
                continue;
            }

            for (size_t i = 1; i + 1 < face.size(); i++) {
                FaceIndex tri[3] = {face[0], face[i], face[i+1]};

                for (int j=0;j<3;j++) {
                    Vert v;

                    v.color = vec4(1.0f);

                    v.position = positions[tri[j].p];

                    if (tri[j].uv >= 0) {
                        v.uv = uvs[tri[j].uv];
                    } else {
                        v.uv = vec2(0.0f);
                    }

                    if (tri[j].n >= 0) {
                        v.normal = normals[tri[j].n];
                    } else {
                        v.normal = vec3(0.0f,1.0f,0.0f);
                    }

                    vertices.push_back(v);
                }
            }
        }
    }

    mesh.upload(vertices);

    std::cout << "Loaded OBJ: " << path << " (" << vertices.size()/3 << " triangles)" << std::endl;

    return mesh;
}