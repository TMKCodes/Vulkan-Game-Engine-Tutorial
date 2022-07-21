#pragma once

#include "../camera.hpp"
#include "../device.hpp"
#include "../frame_info.hpp"
#include "../drawable.hpp"
#include "../pipeline.hpp"

#include <memory>
#include <vector>

namespace tml {
    class SimpleRenderSystem {
        public:
            SimpleRenderSystem(
                TmlDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
            ~SimpleRenderSystem();

            SimpleRenderSystem(const SimpleRenderSystem &) = delete;
            SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

            void renderGameObjects(FrameInfo &frameInfo);

        private:
            void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
            void createPipeline(VkRenderPass renderPass);

            TmlDevice &tmlDevice;

            std::unique_ptr<TmlPipeline> tmlPipeline;
            VkPipelineLayout pipelineLayout;
    };
} 
