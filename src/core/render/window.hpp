#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../math/essentials.hpp"



class Window {
private:
    GLFWwindow* window;

    int width;
    int height;

    std::string title;

    void destroy() {
        if (window != nullptr) {
            glfwDestroyWindow(window);
            window = nullptr;
        }

        glfwTerminate();
    }

    int getKeyID(std::string key) {
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        
        if (key.length() == 1 && key[0] >= 'a' && key[0] <= 'z') {
            return GLFW_KEY_A + (key[0] - 'a');
        }
        if (key.length() == 1 && key[0] >= '0' && key[0] <= '9') {
            return GLFW_KEY_0 + (key[0] - '0');
        }
        
        if (!key.empty() && key[0] == 'f' && key.length() <= 3) {
            int fkey = std::stoi(key.substr(1));
            if (fkey >= 1 && fkey <= 25) {
                return GLFW_KEY_F1 + (fkey - 1);
            }
        }
        
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
        
        return -1;
    }

    int getMouseID(int type) {
        if (type == 0) {
            return GLFW_MOUSE_BUTTON_LEFT;
        } else if (type == 1) {
            return GLFW_MOUSE_BUTTON_MIDDLE;
        } else if (type == 2) {
            return GLFW_MOUSE_BUTTON_RIGHT;
        }

        return GLFW_MOUSE_BUTTON_LEFT;
    }

    static void FramebufferResizeCallback(GLFWwindow* window, int newWidth, int newHeight) {
        Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

        self->width = newWidth;
        self->height = newHeight;

        glViewport(0, 0, newWidth, newHeight);
    }

public:

    float deltaTime;
    vec2 mouse;

    Window(int _width, int _height, std::string title) {
        width = _width;
        height = _height;
        
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(window);

        glewExperimental = GL_TRUE;
        glewInit();
        glViewport(0, 0, width, height);

        glfwSetWindowUserPointer(window, this);

        glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);

        glEnable(GL_DEPTH_TEST);
    }

    ~Window() {
        destroy();
    }

    bool isOpen() {
        return window != nullptr && !glfwWindowShouldClose(window);
    }

    void close() {
        if (window != nullptr) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        destroy();
    }

    void pollEvents() {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float w = static_cast<float>(width);
        float h = static_cast<float>(height);

        mouse = (vec2((float)mouseX, (float)mouseY) - vec2(w * 0.5f, h * 0.5f)) / vec2(w * 0.5f, -h * 0.5f);

        glfwPollEvents();
    }

    void swapBuffers() {
        glfwSwapBuffers(window);
    }

    bool isKeyPressed(std::string key) {
        int k = getKeyID(key);
        return glfwGetKey(window, k) == GLFW_PRESS;
    }

    bool isKeyDown(std::string key) {
        int k = getKeyID(key);
        return glfwGetKey(window, k) == GLFW_REPEAT;
    }

    bool mouseDown(int type) {
        return glfwGetMouseButton(window, getMouseID(type)) == GLFW_PRESS;
    }

    bool mouseDown() {
        return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    }

    GLFWwindow* getNativeWindow();

};