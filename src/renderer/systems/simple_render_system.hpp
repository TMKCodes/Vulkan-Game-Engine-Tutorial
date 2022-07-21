#pragma once

#include "../tml_camera.hpp"
#include "../tml_device.hpp"
#include "../tml_frame_info.hpp"
#include "../tml_game_object.hpp"
#include "../tml_pipeline.hpp"

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