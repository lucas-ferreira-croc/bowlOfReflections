#ifndef BOR_MODEL_HPP
#define BOR_MODEL_HPP

#include "vk/bor_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace bor
{
    class BoRModel
    {
    public:
        struct  Vertex
        {
            glm::vec2 position;
            glm::vec3 color;
            
            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };
        

        BoRModel(BoRDevice& device, const std::vector<Vertex>& vertices);
        ~BoRModel();

        BoRModel(const BoRModel&) = delete;
        BoRModel &operator=(const BoRModel&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);
    private:
        void createVertexBuffers(const std::vector<Vertex>& vertices);

        BoRDevice& borDevice;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
    };
}

#endif