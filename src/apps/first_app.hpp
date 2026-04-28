#ifndef FIRST_APP_HPP
#define FIRST_APP_HPP

#include "window/bor_window.hpp"
#include "vk/bor_device.hpp"
#include "vk/bor_renderer.hpp"

#include "systems/simple_render_system.hpp"

#include "game/bor_game_object.hpp"

#include <memory>
#include <vector>

namespace bor
{
    class FirstApp
    {
    public:
        static constexpr int WIDTH = 2048;
        static constexpr int HEIGHT = 1536;

        FirstApp();
        ~FirstApp();

        FirstApp(const FirstApp&) = delete;
        FirstApp &operator=(const FirstApp&) = delete;

        void run();

    private:
        void loadGameObjects();

        BoRWindow borWindow{WIDTH, HEIGHT, "Bowl of Reflections"};
        BoRDevice borDevice{borWindow};
        BoRRenderer borRenderer{borWindow, borDevice};

        std::vector<BoRGameObject> gameObjects;

    };
}

#endif