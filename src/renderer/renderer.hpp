#pragma once

#include "device.hpp"
#include "swap_chain.hpp"
#include "window.hpp"

#include <cassert>
#include <memory>
#include <vector>

namespace tml {
    class TmlRenderer {
        public:
            TmlRenderer(TmlWindow &window, TmlDevice &device);
            ~TmlRenderer();

            TmlRenderer(const TmlRenderer &) = delete;
            TmlRenderer &operator=(const TmlRenderer &) = delete;

            VkRenderPass getSwapChainRenderPass() const { return tmlSwapChain->getRenderPass(); }
            float getAspectRatio() const { return tmlSwapChain->extentAspectRatio(); }
            bool isFrameInProgress() const { return isFrameStarted; }

            VkCommandBuffer getCurrentCommandBuffer() const {
                assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
                return commandBuffers[currentFrameIndex];
            }

            int getFrameIndex() const {
                assert(isFrameStarted && "Cannot get frame index when frame not in progress");
                return currentFrameIndex;
            }

            VkCommandBuffer beginFrame();
            void endFrame();
            void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
            void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

        private:
            void createCommandBuffers();
            void freeCommandBuffers();
            void recreateSwapChain();

            TmlWindow &tmlWindow;
            TmlDevice &tmlDevice;
            std::unique_ptr<TmlSwapChain> tmlSwapChain;
            std::vector<VkCommandBuffer> commandBuffers;

            uint32_t currentImageIndex;
            int currentFrameIndex{0};
            bool isFrameStarted{false};
    };
}
