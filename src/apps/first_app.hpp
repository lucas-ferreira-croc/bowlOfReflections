#ifndef FIRST_APP_HPP
#define FIRST_APP_HPP

#include "window/bor_window.hpp"
#include "vk/bor_pipeline.hpp"
#include "vk/bor_device.hpp"
#include "vk/bor_swap_chain.hpp"
#include "vk/bor_model.hpp"

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
        void loadModels();

        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void freeCommandBuffers();
        void drawFrame();
        void recreateSwapChain();
        void recordCommandBuffer(int imageIndex);

        BoRWindow borWindow{WIDTH, HEIGHT, "Bowl of Reflections"};
        BoRDevice borDevice{borWindow};
        std::unique_ptr<BoRSwapChain> borSwapChain;
        std::unique_ptr<BoRPipeline> borPipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;

        std::unique_ptr<BoRModel> borModel;
    };
}

#endif