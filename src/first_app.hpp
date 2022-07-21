#pragma once

#include "tml_descriptors.hpp"
#include "tml_device.hpp"
#include "tml_game_object.hpp"
#include "tml_renderer.hpp"
#include "tml_window.hpp"

// std
#include <memory>
#include <vector>

namespace tml {
class FirstApp {
 public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  FirstApp();
  ~FirstApp();

  FirstApp(const FirstApp &) = delete;
  FirstApp &operator=(const FirstApp &) = delete;

  void run();

 private:
  void loadGameObjects();

  TmlWindow tmlWindow{WIDTH, HEIGHT, "Vulkan Tutorial"};
  TmlDevice tmlDevice{tmlWindow};
  TmlRenderer tmlRenderer{tmlWindow, tmlDevice};

  // note: order of declarations matters
  std::unique_ptr<TmlDescriptorPool> globalPool{};
  TmlGameObject::Map gameObjects;
};
}  // namespace tml
