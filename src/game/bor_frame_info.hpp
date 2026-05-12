#ifndef BOR_FRAME_INFO_HPP
#define BOR_FRAME_INFO_HPP

#include "bor_camera.hpp"
#include "bor_game_object.hpp"

#include <vulkan/vulkan.h>

#define MAX_LIGHTS 10

namespace bor
{
    struct PointLight 
    {
        glm::vec4 position{}; // ignore w;
        glm::vec4 color{}; // w is itensity
    };

    struct GlobalUbo
    {
        glm::mat4 projection{1.0f};
        glm::mat4 view{1.0f};
        glm::mat4 inverseView{1.0f};
        glm::vec4 ambientColor{1.0f, 1.0f, 1.0f, 0.02f}; // w is light intensity
        PointLight pointLights[MAX_LIGHTS];
        int numLights;
    };

    struct FrameInfo 
    {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        BoRCamera& camera;
        VkDescriptorSet globalDescriptorSet;
        BoRGameObject::Map& gameObjects;
    };
}

#endif