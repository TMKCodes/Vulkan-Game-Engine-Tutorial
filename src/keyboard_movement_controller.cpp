#include "keyboard_movement_controller.hpp"

#include <limits>
#include <iostream>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace tml {
    void KeyboardMovementController::moveInPlaneXZ(GLFWwindow* window, float dt, TmlDrawable& gameObject) {
        if(!glfwGetWindowAttrib(window, GLFW_HOVERED)) {
            firstClick = true;
            return;
        }
        int width;
        int height;
        glfwGetWindowSize(window, &width, &height);

        if(firstClick) {
            std::cout << "Came out of hibernation. Please play me!" << std::endl;
            firstClick = false;
        }
        double mouseY;
        double mouseX;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float rotateX = lookSpeed * (float)(mouseY - lastCursorPosition.y) / height;
        float rotateY = -(lookSpeed * (float)(mouseX - lastCursorPosition.x) / width);

		glm::vec3 orientation{ .0f, .0f, 6.28318530718f};
        orientation = glm::rotate(orientation, glm::radians(rotateX), glm::vec3{0.f, -1.f, 0.f});
        orientation.x = glm::clamp(orientation.x, -1.57079632679f, 1.57079632679f);
        orientation = glm::rotate(orientation, glm::radians(rotateY), glm::normalize(glm::cross(orientation, glm::vec3{0.f, -1.f, 0.f})));
        orientation.z = glm::two_pi<float>();

        if(glm::dot(orientation, orientation) > std::numeric_limits<float>::epsilon()) {
            gameObject.transform.rotation += orientation;
            std::cout << gameObject.transform.rotation.x << ", " << gameObject.transform.rotation.y << ", " << gameObject.transform.rotation.z << std::endl;
        }

        float yaw = gameObject.transform.rotation.y;
        const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
        const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
        const glm::vec3 upDir{0.f, -1.f, 0.f};

        glm::vec3 moveDir{0.f};
        if(glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
        if(glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
        if(glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
        if(glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
        if(glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
        if(glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

        if(glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
            gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
        }
        // limit pitch values between about +/- 85ish degrees
        gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
        gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());
        gameObject.transform.rotation.z = 0;
        lastCursorPosition.x = mouseX;
        lastCursorPosition.y = mouseY;
        //glfwSetCursorPos(window, width / 2, height / 2);

    }
} 