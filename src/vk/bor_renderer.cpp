#include "bor_renderer.hpp"

#include <stdexcept>
#include <array>

namespace bor
{
    BoRRenderer::BoRRenderer(BoRWindow& window, BoRDevice& device)
        : borWindow{window}, borDevice{device}
    {
        recreateSwapChain();
        createCommandBuffers();
    }
    
    BoRRenderer::~BoRRenderer()
    {
        freeCommandBuffers();
    }

    VkCommandBuffer BoRRenderer::beginFrame()
    {
        assert(!isFrameStarted && "cant call beginFrame while already in progress");
        auto result = borSwapChain->acquireNextImage(&currentImageIndex);

        if(result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return nullptr;
        }

        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw::std::runtime_error("failed to acquire swap chain image");
        }

        isFrameStarted = true;
        auto commandBuffer = getCurrentCommandBuffer();

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            std::runtime_error("failed to begin recording command buffer");
        }

        return commandBuffer;
    }
    
    void BoRRenderer::endFrame()
    {
        assert(isFrameInProgress() && "cannot call endframe while frame is not in progress");

        auto commandBuffer = getCurrentCommandBuffer();
        
        if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            throw::std::runtime_error("failed to record command buffer");
        }

        auto result = borSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
        if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || borWindow.wasWindowResized())
        {
            borWindow.resetWindowResizedFlag();
            recreateSwapChain();
        }
        else if(result != VK_SUCCESS)
        {
            throw::std::runtime_error("failed to present swap chain image");
        }

        isFrameStarted = false;
        currentFrameIndex = (currentFrameIndex + 1) % BoRSwapChain::MAX_FRAMES_IN_FLIGHT;
    }
    void BoRRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert(isFrameInProgress() && "cant call beginSwapChainRenderPass if frame is not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "cant begin render pass in a command buffer of a different frame");

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            std::runtime_error("failed to begin recording command buffer");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = borSwapChain->getRenderPass();
        renderPassInfo.framebuffer = borSwapChain->getFrameBuffer(currentImageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = borSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f };
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(borSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(borSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, borSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    }

    void BoRRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert(isFrameInProgress() && "cant call endSwapChainRenderPass if frame is not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "cant end render pass in a command buffer of a different frame");

        vkCmdEndRenderPass(commandBuffer);
        
    }

    void BoRRenderer::createCommandBuffers()
    {
      commandBuffers.resize(BoRSwapChain::MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = borDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if(vkAllocateCommandBuffers(borDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
        {
            std::runtime_error("failed to allocate command buffers");
        }
    }

    void BoRRenderer::freeCommandBuffers()
    {
        vkFreeCommandBuffers(borDevice.device(), borDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
        commandBuffers.clear();
    }
    
    void BoRRenderer::recreateSwapChain()
    {
        auto extent = borWindow.getExtent();
        
        while(extent.width == 0 || extent.height == 0)
        {
            extent = borWindow.getExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(borDevice.device());

        if(borSwapChain == nullptr)
        {
            borSwapChain = std::make_unique<BoRSwapChain>(borDevice, extent);
        }
        else
        {
            std::shared_ptr<BoRSwapChain> oldSwapChain = std::move(borSwapChain);
            borSwapChain = std::make_unique<BoRSwapChain>(borDevice, extent, oldSwapChain);

            if(!oldSwapChain->compareSwapFromats(*borSwapChain.get()))
            {
                throw std::runtime_error("swap chain image(or depth) format has changed");
            }
        }
    }

}