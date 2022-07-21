#pragma once

#include "renderer/descriptors.hpp"
#include "renderer/device.hpp"
#include "renderer/drawable.hpp"
#include "renderer/renderer.hpp"
#include "renderer/window.hpp"

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

            TmlWindow tmlWindow{WIDTH, HEIGHT, true, "Vulkan Tutorial"};
            TmlDevice tmlDevice{tmlWindow};
            TmlRenderer tmlRenderer{tmlWindow, tmlDevice};

            // note: order of declarations matters
            std::unique_ptr<TmlDescriptorPool> globalPool{};
            TmlDrawable::Map gameObjects;
    };
}
