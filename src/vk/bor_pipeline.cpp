#include "bor_pipeline.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>
namespace bor
{
    BoRPipeline::BoRPipeline(BoRDevice& device,  const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo)
        : borDevice(device)
    {
        createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
    }


    std::vector<char> BoRPipeline::readFile(const std::string& filepath)
    {
        std::ifstream file{filepath, std::ios::ate | std::ios::binary};
        if(!file.is_open())
        {
            throw new std::runtime_error("failed to open file: " + filepath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    void BoRPipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if(vkCreateShaderModule(borDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
        {
            throw new std::runtime_error("failed to create shader module");
        }
    }

    void BoRPipeline::createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo)
    {
        auto vertCode = readFile(vertFilepath);
        auto fragCode = readFile(fragFilepath);

        std::cout << "Vertex Shader Code size: " << vertCode.size() << '\n';
        std::cout << "Fragment Shader Code size: " << fragCode.size() << '\n';
    }


    PipelineConfigInfo BoRPipeline::defaultPipelineConfigInfo(uint32_t widht, uint32_t height)
    {
        PipelineConfigInfo configInfo{};
        return configInfo;
    }

}
