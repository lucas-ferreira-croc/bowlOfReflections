#include "systems/point_light_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


#include <stdexcept>

namespace bor
{
    BoRPointLightSystem::BoRPointLightSystem(BoRDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
        : borDevice{device}
    {
        createPipelineLayout(globalSetLayout);
        createPipeline(renderPass);
    }

    BoRPointLightSystem::~BoRPointLightSystem()
    {
        vkDestroyPipelineLayout(borDevice.device(), pipelineLayout, nullptr);
    }

    void BoRPointLightSystem::render(FrameInfo frameInfo)
    {
        borPipeline->bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(
            frameInfo.commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipelineLayout,
            0,
            1,
            &frameInfo.globalDescriptorSet,
            0,
            nullptr
        );

        vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
    }

    void BoRPointLightSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
    {
        // VkPushConstantRange pushConstantRange{};
        // pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        // pushConstantRange.offset = 0;
        // pushConstantRange.size = sizeof(SimplePushConstantData);
        
        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if(vkCreatePipelineLayout(borDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to craete pipeline layout");
        }
    }

    void BoRPointLightSystem::createPipeline(VkRenderPass renderPass)
    {
         assert(pipelineLayout != nullptr && "cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfigInfo{};
        BoRPipeline::defaultPipelineConfigInfo(pipelineConfigInfo);
        pipelineConfigInfo.attributeDescription.clear();
        pipelineConfigInfo.bindingDescription.clear();
        
        pipelineConfigInfo.renderPass = renderPass;
        pipelineConfigInfo.pipelineLayout = pipelineLayout;
        borPipeline = std::make_unique<BoRPipeline>(borDevice,
             "C:\\dev\\bowlOfReflections\\shaders\\point_light.vert.spv", "C:\\dev\\bowlOfReflections\\shaders\\point_light.frag.spv",
              pipelineConfigInfo);
    }
}