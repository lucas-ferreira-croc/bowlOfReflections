#ifndef BOR_PIPELINE_HPP
#define BOR_PIPELINE_HPP

#include "bor_device.hpp"

#include <string>
#include <vector>

namespace bor
{

    struct PipelineConfigInfo{};

    class BoRPipeline
    {
    public:
        BoRPipeline(BoRDevice& device,  const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);
        ~BoRPipeline() {}

        BoRPipeline(const BoRPipeline&) = delete;
        BoRPipeline &operator=(const BoRPipeline&) = delete;

        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t widht, uint32_t height);

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