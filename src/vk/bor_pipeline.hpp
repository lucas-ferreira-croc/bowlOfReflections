#ifndef BOR_PIPELINE_HPP
#define BOR_PIPELINE_HPP

#include "bor_device.hpp"

#include <string>
#include <vector>

namespace bor
{

    struct PipelineConfigInfo
    {
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class BoRPipeline
    {
    public:
        BoRPipeline(BoRDevice& device,  const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);
        ~BoRPipeline();

        BoRPipeline(const BoRPipeline&) = delete;
        BoRPipeline &operator=(const BoRPipeline&) = delete;

        void bind(VkCommandBuffer commandBuffer);

        static PipelineConfigInfo defaultPipelineConfigInfo(PipelineConfigInfo& configInfo, uint32_t widht, uint32_t height);

    private:
        static std::vector<char> readFile(const std::string& filepath);
        void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, 
            const PipelineConfigInfo& pipelineConfigInfo);

        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

        BoRDevice& borDevice;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
};

#endif