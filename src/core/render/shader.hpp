#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../math/essentials.hpp"


struct Uniform {
    GLchar* name;
    GLint location;
};


class Shader {
private:
    std::string vert = "shaders/vert.glsl";
    std::string frag = "shaders/frag.glsl";

    std::string vertCode;
    std::string fragCode;

    GLuint vertShader;
    GLuint fragShader;

    GLuint program;

    std::vector<Uniform> locations;

    std::string openFile(std::string path) {
        std::ifstream file(path);

        if (file.is_open()) {
            std::stringstream stream;
            stream << file.rdbuf();

            std::string source = stream.str();

            return source;
        }

        return "Failed to load file";
    }

    void cache(GLchar* name, GLint location) {
        Uniform u;
        u.name = name;
        u.location = location;

        locations.push_back(u);
    }

    GLint checkSaved(GLchar* name) {
        GLint result;
        for (int i = 0; i < locations.size(); i++) {
            Uniform u = locations[i];
            if (u.name == name) {
                result = u.location;
                break;
            }
        }

        return result;
    }

public:

    void load() {
        vertShader = glCreateShader(GL_VERTEX_SHADER);
        vertCode = openFile(vert);

        const char* vertSource = vertCode.c_str();
        glShaderSource(vertShader, 1, &vertsource, nullptr);
        glCompileShader(vertShader);


        fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        fragCode = openFile(frag);

        const char* fragSource = fragCode.c_str();
        glShaderSource(fragShader, 1, &fragsource, nullptr);
        glCompileShader(fragShader);


        program = glCreateProgram();
        
        glAttachShader(program, vertShader);
        glAttachShader(program, fragShader);

        glLinkProgram(program);

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
    }

    void load(std::string _vert, std::string _frag) {
        vert = _vert;
        frag = _frag;

        load();
    }

    void bind() {
        glUseProgram(program);
    }

    void setUniform(GLchar* name, float inp) {
        GLint l = checkSaved(name);
        if (!l) {
            GLint location = glGetUniformLocation(program, name);
            cache(name, location);
            l = location;
        }
        glUniform1f(l, inp);
    }

    void setUniform(GLchar* name, vec2 inp) {
        GLint l = checkSaved(name);
        if (!l) {
            GLint location = glGetUniformLocation(program, name);
            cache(name, location);
            l = location;
        }
        glUniform2f(l, inp.x, inp.y);
    }

    void setUniform(GLchar* name, vec3 inp) {
        GLint l = checkSaved(name);
        if (!l) {
            GLint location = glGetUniformLocation(program, name);
            cache(name, location);
            l = location;
        }
        glUniform3f(l, inp.x, inp.y, inp.z);
    }

    void setUniform(GLchar* name, vec4 inp) {
        GLint l = checkSaved(name);
        if (!l) {
            GLint location = glGetUniformLocation(program, name);
            cache(name, location);
            l = location;
        }
        glUniform4f(l, inp.x, inp.y, inp.z, inp.w);
    }

    void setUniform(GLchar* name, int inp) {
        GLint l = checkSaved(name);
        if (!l) {
            GLint location = glGetUniformLocation(program, name);
            cache(name, location);
            l = location;
        }
        glUniform1i(l, inp);
    }
};