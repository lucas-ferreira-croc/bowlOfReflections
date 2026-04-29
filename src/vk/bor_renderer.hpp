#ifndef BOR_RENDERER_HPP
#define BOR_RENDERER_HPP

#include "window/bor_window.hpp"
#include "vk/bor_device.hpp"
#include "vk/bor_swap_chain.hpp"

#include <memory>
#include <vector>
#include <cassert>

namespace bor
{
    class BoRRenderer
    {
    public:

        BoRRenderer(BoRWindow& window, BoRDevice& device);
        ~BoRRenderer();

        BoRRenderer(const BoRRenderer&) = delete;
        BoRRenderer &operator=(const BoRRenderer&) = delete;

        VkRenderPass getSwapChainRenderPass() const {
            return borSwapChain->getRenderPass();
        }

        float getAspectRatio() const { return borSwapChain->extentAspectRatio(); }

        bool isFrameInProgress() const { return isFrameStarted; };
        VkCommandBuffer getCurrentCommandBuffer() const {
            assert(isFrameStarted && "cannot get command buffer when frame not in progress");
            return commandBuffers[currentFrameIndex];
        }
        
        int getFrameIndex() const
        {
            assert(isFrameStarted && "cannot get frame index when frame not in progress");
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

        BoRWindow& borWindow;
        BoRDevice& borDevice;
        std::unique_ptr<BoRSwapChain> borSwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex{0};
        bool isFrameStarted{false};
    };
}

#endif