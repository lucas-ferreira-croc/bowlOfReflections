#include "first_app.hpp"

#include <stdexcept>
#include <array>
#include <iostream>

namespace bor
{
    FirstApp::FirstApp()
    {
        loadModels();
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    FirstApp::~FirstApp()
    {
        vkDestroyPipelineLayout(borDevice.device(), pipelineLayout, nullptr);
    }

    void FirstApp::loadModels()
    {
        bool useSierpinski = true;

        std::vector<BoRModel::Vertex> vertices;

        std::function<void(int, glm::vec2, glm::vec2, glm::vec2)> sierpinski;
        sierpinski = [&](int depth, glm::vec2 left, glm::vec2 right, glm::vec2 top)
        {
            if (depth <= 0)
            {
                vertices.push_back({top});
                vertices.push_back({right});
                vertices.push_back({left});
            }
            else
            {
                auto leftTop = 0.5f * (left + top);
                auto rightTop = 0.5f * (right + top);
                auto leftRight = 0.5f * (left + right);

                sierpinski(depth - 1, left, leftRight, leftTop);
                sierpinski(depth - 1, leftRight, right, rightTop);
                sierpinski(depth - 1, leftTop, rightTop, top);
            }
        };
        
        if(useSierpinski)
        {
            sierpinski(1, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});
        }
        else 
        {
            vertices = {
                {{0.0f, -0.5f}},
                {{0.5f,  0.5f}},
                {{-0.5f, 0.5f}},
            };
        }
        borModel = std::make_unique<BoRModel>(borDevice, vertices);
    }

    void FirstApp::run()
    {
        while (!borWindow.shouldClose())
        {
            glfwPollEvents();
            drawFrame();
        }   

        vkDeviceWaitIdle(borDevice.device());
    }

    void FirstApp::createPipelineLayout()
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if(vkCreatePipelineLayout(borDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to craete pipeline layout");
        }
    }

    void FirstApp::createPipeline()
    {
        PipelineConfigInfo pipelineConfig{};
        auto pipelineConfigInfo = BoRPipeline::defaultPipelineConfigInfo(pipelineConfig, borSwapChain.width(), borSwapChain.height());
        pipelineConfigInfo.renderPass = borSwapChain.getRenderPass();
        pipelineConfigInfo.pipelineLayout = pipelineLayout;
        borPipeline = std::make_unique<BoRPipeline>(borDevice,
             "C:\\dev\\bowlOfReflections\\shaders\\simple_shader.vert.spv", "C:\\dev\\bowlOfReflections\\shaders\\simple_shader.frag.spv",
              pipelineConfigInfo);
    }

    void FirstApp::createCommandBuffers()
    {
        commandBuffers.resize(borSwapChain.imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = borDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if(vkAllocateCommandBuffers(borDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
        {
            std::runtime_error("failed to allocate command buffers");
        }

        std::cout << " commandBuffers.size(): " <<  commandBuffers.size() << '\n';
        for(int i = 0; i < commandBuffers.size(); ++i)
        {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
            {
                std::runtime_error("failed to begin recording command buffer");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = borSwapChain.getRenderPass();
            renderPassInfo.framebuffer = borSwapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = borSwapChain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.4f, 0.6f, 0.9f, 1.0f };
            clearValues[1].depthStencil = {1.0f, 0};
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            borPipeline->bind(commandBuffers[i]);
            
            borModel->bind(commandBuffers[i]);
            borModel->draw(commandBuffers[i]);

            vkCmdEndRenderPass(commandBuffers[i]);
            if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
            {
                throw::std::runtime_error("failed to record command buffer");
            }
        }
    }

    void FirstApp::drawFrame()
    {
        uint32_t imageIndex;
        auto result = borSwapChain.acquireNextImage(&imageIndex);

        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw::std::runtime_error("failed to acquire swap chain image");
        }

        result = borSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if(result != VK_SUCCESS)
        {
            throw::std::runtime_error("failed to present swap chain image");
        }
    }

}