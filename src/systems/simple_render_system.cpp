#include "systems/simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


#include <stdexcept>

namespace bor
{
    struct SimplePushConstantData
    {
        glm::mat2 transform{1.0f};
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };


    BoRSimpleRenderSystem::BoRSimpleRenderSystem(BoRDevice& device, VkRenderPass renderPass)
        : borDevice{device}
    {
        createPipelineLayout();
        createPipeline(renderPass);
    }

    BoRSimpleRenderSystem::~BoRSimpleRenderSystem()
    {
        vkDestroyPipelineLayout(borDevice.device(), pipelineLayout, nullptr);
    }

    void BoRSimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<BoRGameObject>& gameObjects)
    {
        borPipeline->bind(commandBuffer);
        for(auto& obj : gameObjects)
        {
            obj.transform2D.rotation = glm::mod(obj.transform2D.rotation + 0.01f, glm::tau<float>());

            SimplePushConstantData push{};
            push.offset = obj.transform2D.translation;
            push.color = obj.color;
            push.transform = obj.transform2D.mat2();

            vkCmdPushConstants(
                commandBuffer,
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData),
                &push
            );

            obj.model->bind(commandBuffer);
            obj.model->draw(commandBuffer);
        }
    }

    void BoRSimpleRenderSystem::createPipelineLayout()
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);
        
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if(vkCreatePipelineLayout(borDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to craete pipeline layout");
        }
    }

    void BoRSimpleRenderSystem::createPipeline(VkRenderPass renderPass)
    {
         assert(pipelineLayout != nullptr && "cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfigInfo{};
        BoRPipeline::defaultPipelineConfigInfo(pipelineConfigInfo);
        pipelineConfigInfo.renderPass = renderPass;
        pipelineConfigInfo.pipelineLayout = pipelineLayout;
        borPipeline = std::make_unique<BoRPipeline>(borDevice,
             "C:\\dev\\bowlOfReflections\\shaders\\simple_shader.vert.spv", "C:\\dev\\bowlOfReflections\\shaders\\simple_shader.frag.spv",
              pipelineConfigInfo);
    }
}