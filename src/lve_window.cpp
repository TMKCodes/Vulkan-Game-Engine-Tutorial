#include "tml_window.hpp"

// std
#include <stdexcept>

namespace tml {

TmlWindow::TmlWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
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

  window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void TmlWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
  if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
    throw std::runtime_error("failed to craete window surface");
  }
}

void TmlWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
  auto tmlWindow = reinterpret_cast<TmlWindow *>(glfwGetWindowUserPointer(window));
  tmlWindow->framebufferResized = true;
  tmlWindow->width = width;
  tmlWindow->height = height;
}

}  // namespace tml
