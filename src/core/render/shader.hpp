#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "../math/essentials.hpp"


struct Uniform {
    GLchar* name;
    GLint location;
};

struct Texture {
    std::string path;
    GLuint texture;
};


class Shader {
private:
    std::string vert = "src/core/render/shaders/game/vert.glsl";
    std::string frag = "src/core/render/shaders/game/frag.glsl";

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
        for (size_t i = 0; i < locations.size(); i++) {
            if (strcmp(locations[i].name, name) == 0) {
                return locations[i].location;
            }
        }

        return -1;
    }

    GLuint getTex(std::string path) {
        GLuint result;
        for (size_t i = 0; i < textures.size(); i++) {
            Texture t = textures[i];
            if (t.path == path) {
                result = t.texture;
                break;
            }
        }

        return result;
    }

    GLuint loadTexture(const char* path) {
        int width, height, channels;

        stbi_set_flip_vertically_on_load(true);

        unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

        if (!data) {
            std::cout << "Failed to load image: " << path << std::endl;
            return 0;
        }

        GLenum format;

        if (channels == 1) {
            format = GL_RED;
        } else if (channels == 3) {
            format = GL_RGB;
        } else if (channels == 4) {
            format = GL_RGBA;
        } else {
            stbi_image_free(data);
            return 0;
        }

        GLuint texture;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        stbi_image_free(data);

        return texture;
    }


public:
    std::vector<Texture> textures;

    void load() {
        vertShader = glCreateShader(GL_VERTEX_SHADER);
        vertCode = openFile(vert);

        const char* vertSource = vertCode.c_str();
        glShaderSource(vertShader, 1, &vertSource, nullptr);
        glCompileShader(vertShader);

        GLint success;
        glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char log[1024];
            glGetShaderInfoLog(vertShader, 1024, NULL, log);
            std::cout << "Vertex Shader:\n" << log << std::endl;
        }


        fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        fragCode = openFile(frag);

        const char* fragSource = fragCode.c_str();
        glShaderSource(fragShader, 1, &fragSource, nullptr);
        glCompileShader(fragShader);

        program = glCreateProgram();
        
        glAttachShader(program, vertShader);
        glAttachShader(program, fragShader);

        glLinkProgram(program);

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
    }

    void load(std::string _vert, std::string _frag) {
        vert = "src/core/render/shaders/" + _vert;
        frag = "src/core/render/shaders/" + _frag;

        load();
    }

    void bind() {
        glUseProgram(program);
    }

    void addTex(const char* path) {
        GLuint texture = loadTexture(path);
        textures.push_back({path, texture});

        glUseProgram(program);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    void setUniform(GLchar* name, float inp) {
        GLint l = checkSaved((GLchar*)name);
        if (l == -1) {
            GLint location = glGetUniformLocation(program, (GLchar*)name);
            cache((GLchar*)name, location);
            l = location;
        }
        glUniform1f(l, inp);
    }

    void setUniform(GLchar* name, double inp) {
        GLint l = checkSaved((GLchar*)name);
        if (l == -1) {
            GLint location = glGetUniformLocation(program, (GLchar*)name);
            cache((GLchar*)name, location);
            l = location;
        }
        glUniform1f(l, (float)inp);
    }

    void setUniform(GLchar* name, vec2 inp) {
        GLint l = checkSaved((GLchar*)name);
        if (l == -1) {
            GLint location = glGetUniformLocation(program, (GLchar*)name);
            cache((GLchar*)name, location);
            l = location;
        }
        glUniform2f(l, inp.x, inp.y);
    }

    void setUniform(GLchar* name, vec3 inp) {
        GLint l = checkSaved((GLchar*)name);
        if (l == -1) {
            GLint location = glGetUniformLocation(program, (GLchar*)name);
            cache((GLchar*)name, location);
            l = location;
        }
        glUniform3f(l, inp.x, inp.y, inp.z);
    }

    void setUniform(GLchar* name, vec4 inp) {
        GLint l = checkSaved((GLchar*)name);
        if (l == -1) {
            GLint location = glGetUniformLocation(program, (GLchar*)name);
            cache((GLchar*)name, location);
            l = location;
        }
        glUniform4f(l, inp.x, inp.y, inp.z, inp.w);
    }

    void setUniform(GLchar* name, int inp) {
        GLint l = checkSaved((GLchar*)name);
        if (l == -1) {
            GLint location = glGetUniformLocation(program, (GLchar*)name);
            cache((GLchar*)name, location);
            l = location;
        }
        glUniform1i(l, inp);
    }

    void setTexture(GLchar* uniform, GLuint texture, GLint unit = 0) {
        bind();

        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, texture);

        GLint l = checkSaved(uniform);
        if (l == -1)
        {
            l = glGetUniformLocation(program, uniform);
            cache(uniform, l);
        }

        glUniform1i(l, unit);
    }

    void setTexture(GLchar* uniform, const std::string& path, GLint unit = 0) {
        setTexture(uniform, getTex(path), unit);
    }
};