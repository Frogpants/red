#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../math/essentials.hpp"


struct Vertex {
    vec2 pos;
    vec4 color = vec4(0.0);
    vec2 uv;
};

struct Tri {
    Vertex v1;
    Vertex v2;
    Vertex v3;
};



class Window {
private:
    GLFWwindow* window;

    int width;
    int height;

    std::string title;

    int getKeyID(std::string key) {
        // Convert to lowercase for case-insensitive comparison
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        
        // Alphanumeric keys
        if (key.length() == 1 && key[0] >= 'a' && key[0] <= 'z') {
            return GLFW_KEY_A + (key[0] - 'a');
        }
        if (key.length() == 1 && key[0] >= '0' && key[0] <= '9') {
            return GLFW_KEY_0 + (key[0] - '0');
        }
        
        // Function keys
        if (key.substr(0, 2) == "f" && key.length() <= 3) {
            int fkey = std::stoi(key.substr(1));
            if (fkey >= 1 && fkey <= 25) {
                return GLFW_KEY_F1 + (fkey - 1);
            }
        }
        
        // Named keys
        if (key == "escape") return GLFW_KEY_ESCAPE;
        if (key == "esc") return GLFW_KEY_ESCAPE;
        if (key == "enter" || key == "return") return GLFW_KEY_ENTER;
        if (key == "tab") return GLFW_KEY_TAB;
        if (key == "backspace") return GLFW_KEY_BACKSPACE;
        if (key == "space") return GLFW_KEY_SPACE;
        if (key == "delete") return GLFW_KEY_DELETE;
        if (key == "insert") return GLFW_KEY_INSERT;
        if (key == "home") return GLFW_KEY_HOME;
        if (key == "end") return GLFW_KEY_END;
        if (key == "pageup") return GLFW_KEY_PAGE_UP;
        if (key == "pagedown") return GLFW_KEY_PAGE_DOWN;
        if (key == "up") return GLFW_KEY_UP;
        if (key == "down") return GLFW_KEY_DOWN;
        if (key == "left") return GLFW_KEY_LEFT;
        if (key == "right") return GLFW_KEY_RIGHT;
        if (key == "shift") return GLFW_KEY_LEFT_SHIFT;
        if (key == "control" || key == "ctrl") return GLFW_KEY_LEFT_CONTROL;
        if (key == "alt") return GLFW_KEY_LEFT_ALT;
        if (key == "caps") return GLFW_KEY_CAPS_LOCK;
        if (key == "numlock") return GLFW_KEY_NUM_LOCK;
        
        // Return -1 if key not found
        return -1;
    }

public:

    float deltaTime;

    Window(int width, int height, std::string title) {
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(window);

        glewExperimental = GL_TRUE;
        glewInit();
        glViewport(0, 0, width, height);

        glEnable(GL_DEPTH_TEST);
    }

    ~Window() {
        close();
    }

    bool isOpen() {
        return !glfwWindowShouldClose(window);
    }

    void close() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void pollEvents() {
        glfwPollEvents();
    }

    void swapBuffers() {
        glfwSwapBuffers(window);
    }

    bool isKeyPressed(std::string key) {
        int k = getKeyID(key);
        return glfwGetKey(window, k) == GLFW_PRESS;
    }

    GLFWwindow* getNativeWindow();

};