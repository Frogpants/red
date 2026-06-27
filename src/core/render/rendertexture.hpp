#pragma once

#include <iostream>

#include <GL/glew.h>

#include "shader.hpp"

struct RenderTexture {
    GLuint fbo = 0;
    GLuint texture = 0;
    GLuint depth = 0;

    Shader triShader;
    Shader postShader;

    int width = 0;
    int height = 0;

    void create(int w, int h, std::string triVert, std::string triFrag, std::string postVert, std::string postFrag) {
        width = w;
        height = h;

        triShader.load(triVert, triFrag);
        postShader.load(postVert, postFrag);

        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

        glGenRenderbuffers(1, &depth);
        glBindRenderbuffer(GL_RENDERBUFFER, depth);

        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer incomplete\n";

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0,0,width,height);
    }

    static void bindDefault(int width,int height) {
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glViewport(0,0,width,height);
    }

    GLuint getTexture() const {
        return texture;
    }
};