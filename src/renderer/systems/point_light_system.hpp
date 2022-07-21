#pragma once

#include "../camera.hpp"
#include "../device.hpp"
#include "../frame_info.hpp"
#include "../drawable.hpp"
#include "../pipeline.hpp"

#include <memory>
#include <vector>

namespace tml {
    class PointLightSystem {
        public:
            PointLightSystem(
                TmlDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
            ~PointLightSystem();
            PointLightSystem(const PointLightSystem &) = delete;
            PointLightSystem &operator=(const PointLightSystem &) = delete;
            void update(FrameInfo &frameInfo, GlobalUbo &ubo);
            void render(FrameInfo &frameInfo);

        private:
            void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
            void createPipeline(VkRenderPass renderPass);
            TmlDevice &tmlDevice;
            std::unique_ptr<TmlPipeline> tmlPipeline;
            VkPipelineLayout pipelineLayout;
    };
}
