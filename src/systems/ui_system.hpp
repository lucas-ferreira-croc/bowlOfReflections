#ifndef BOR_UI_RENDER_SYSTEM_HPP
#define BOR_UI_RENDER_SYSTEM_HPP

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include "vk/bor_device.hpp"
#include "vk/bor_renderer.hpp"

namespace bor
{
    class BoRUInterfaceSystem
    {
    public:
        BoRUInterfaceSystem(BoRWindow& window, BoRDevice& device, VkRenderPass renderPass);

        void newFrame();
        void render(VkCommandBuffer commandBuffer);
    private:
        ImGuiIO io;
    };
}


#endif