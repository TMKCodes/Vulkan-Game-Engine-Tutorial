#pragma once

#include "../camera.hpp"
#include "../device.hpp"
#include "../frame_info.hpp"
#include "../game_object.hpp"
#include "../pipeline.hpp"

// std
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
}  // namespace tml
