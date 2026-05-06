#ifndef BOR_FRAME_INFO_HPP
#define BOR_FRAME_INFO_HPP

#include "bor_camera.hpp"

#include <vulkan/vulkan.h>

namespace bor
{
    struct FrameInfo 
    {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        BoRCamera& camera;
        VkDescriptorSet globalDescriptorSet;
    };
}

#endif