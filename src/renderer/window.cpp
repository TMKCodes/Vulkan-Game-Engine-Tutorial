#include "window.hpp"

#include <stdexcept>

namespace tml {
    TmlWindow::TmlWindow(int w, int h, bool fullscreen, std::string name) : width{w}, height{h}, fullscreen{fullscreen}, windowName{name} {
        initWindow();
    }
    TmlWindow::~TmlWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    void TmlWindow::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);

        if(fullscreen) {
            window = glfwCreateWindow(glfwGetVideoMode(glfwGetPrimaryMonitor())->width,
                                            glfwGetVideoMode(glfwGetPrimaryMonitor())->height, "Too Much Lost Engine",
                                            glfwGetPrimaryMonitor(), nullptr);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            if(glfwRawMouseMotionSupported()) {
                glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
            }
        } else {
            window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            if(glfwRawMouseMotionSupported()) {
                glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
            }
        }
        
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }
    void TmlWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("Failed to craete window surface.");
        }
    }
    void TmlWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
        auto tmlWindow = reinterpret_cast<TmlWindow *>(glfwGetWindowUserPointer(window));
        tmlWindow->framebufferResized = true;
        tmlWindow->width = width;
        tmlWindow->height = height;
    }
}
