#include "first_app.hpp"

#include "keyboard_movement_controller.hpp"
#include "renderer/buffer.hpp"
#include "renderer/camera.hpp"
#include "renderer/systems/point_light_system.hpp"
#include "renderer/systems/simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>
#include <iostream>

namespace tml {

    FirstApp::FirstApp() {
        globalPool =
            TmlDescriptorPool::Builder(tmlDevice)
                .setMaxSets(TmlSwapChain::MAX_FRAMES_IN_FLIGHT)
                .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, TmlSwapChain::MAX_FRAMES_IN_FLIGHT)
                .build();
        loadGameObjects();
    }

    FirstApp::~FirstApp() {}

    void FirstApp::run() {
        std::vector<std::unique_ptr<TmlBuffer>> uboBuffers(TmlSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); i++) {
            uboBuffers[i] = std::make_unique<TmlBuffer>(
                tmlDevice,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uboBuffers[i]->map();
        }

        auto globalSetLayout =
            TmlDescriptorSetLayout::Builder(tmlDevice)
                .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
                .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(TmlSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            TmlDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }
        SimpleRenderSystem simpleRenderSystem{
            tmlDevice,
            tmlRenderer.getSwapChainRenderPass(),
            globalSetLayout->getDescriptorSetLayout()};
        PointLightSystem pointLightSystem{
            tmlDevice,
            tmlRenderer.getSwapChainRenderPass(),
            globalSetLayout->getDescriptorSetLayout()};
        TmlCamera camera{};

        auto viewerObject = TmlDrawable::createGameObject();
        viewerObject.transform.translation.z = -2.5f;
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::steady_clock::now();
        while (!tmlWindow.shouldClose()) {
            glfwPollEvents();
            auto newTime = std::chrono::steady_clock::now();
            float frameTime =
                std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.moveInPlaneXZ(tmlWindow.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = tmlRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(90.f), aspect, 0.1f, 150.f);

            if (auto commandBuffer = tmlRenderer.beginFrame()) {
                int frameIndex = tmlRenderer.getFrameIndex();
                FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                    globalDescriptorSets[frameIndex],
                    gameObjects};

                // update
                GlobalUbo ubo{};
                ubo.projection = camera.getProjection();
                ubo.view = camera.getView();
                ubo.inverseView = camera.getInverseView();
                pointLightSystem.update(frameInfo, ubo);
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // render
                tmlRenderer.beginSwapChainRenderPass(commandBuffer);

                // order here matters
                simpleRenderSystem.renderGameObjects(frameInfo);
                pointLightSystem.render(frameInfo);

                tmlRenderer.endSwapChainRenderPass(commandBuffer);
                tmlRenderer.endFrame();
            }
        }
        vkDeviceWaitIdle(tmlDevice.device());
    }

    void FirstApp::loadGameObjects() {
        std::shared_ptr<TmlModel> tmlModel =
            TmlModel::createModelFromFile(tmlDevice, "models/flat_vase.obj");
        auto flatVase = TmlDrawable::createGameObject();
        flatVase.model = tmlModel;
        flatVase.transform.translation = {-.5f, .5f, 0.f};
        flatVase.transform.scale = {3.f, 1.5f, 3.f};
        gameObjects.emplace(flatVase.getId(), std::move(flatVase));

        tmlModel = TmlModel::createModelFromFile(tmlDevice, "models/smooth_vase.obj");
        auto smoothVase = TmlDrawable::createGameObject();
        smoothVase.model = tmlModel;
        smoothVase.transform.translation = {.5f, .5f, 0.f};
        smoothVase.transform.scale = {3.f, 1.5f, 3.f};
        gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));

        tmlModel = TmlModel::createModelFromFile(tmlDevice, "models/quad.obj");
        auto floor = TmlDrawable::createGameObject();
        floor.model = tmlModel;
        floor.transform.translation = {0.f, .5f, 0.f};
        floor.transform.scale = {3.f, 1.f, 3.f};
        gameObjects.emplace(floor.getId(), std::move(floor));

        std::vector<glm::vec3> lightColors{
            {1.f, .1f, .1f},
            {.1f, .1f, 1.f},
            {.1f, 1.f, .1f},
            {1.f, 1.f, .1f},
            {.1f, 1.f, 1.f},
            {1.f, 1.f, 1.f}  //
        };

        for (int i = 0; i < lightColors.size(); i++) {
            auto pointLight = TmlDrawable::makePointLight(0.2f);
            pointLight.color = lightColors[i];
            auto rotateLight = glm::rotate(
                glm::mat4(1.f),
                (i * glm::two_pi<float>()) / lightColors.size(),
                {0.f, -1.f, 0.f});
            pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
            gameObjects.emplace(pointLight.getId(), std::move(pointLight));
        }
    }

}  // namespace tml
