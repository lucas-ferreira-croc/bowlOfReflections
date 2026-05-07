#ifndef BOR_POINT_LIGHT_SYSTEM_HPP
#define BOR_POINT_LIGHT_SYSTEM_HPP

#include "game/bor_camera.hpp"
#include "vk/bor_pipeline.hpp"
#include "vk/bor_device.hpp"

#include "game/bor_game_object.hpp"
#include "game/bor_frame_info.hpp"

#include <memory>
#include <vector>

namespace bor
{
    class BoRPointLightSystem
    {
    public:
        BoRPointLightSystem() = default;
        BoRPointLightSystem(BoRDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~BoRPointLightSystem();

        BoRPointLightSystem(const BoRPointLightSystem&) = delete;
        BoRPointLightSystem &operator=(const BoRPointLightSystem&) = delete;

        void update(FrameInfo& frameInfo, GlobalUbo& ubo);
        void render(FrameInfo& frameInfo);
    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        BoRDevice& borDevice;
          
        std::unique_ptr<BoRPipeline> borPipeline;
        VkPipelineLayout pipelineLayout;


    };
}

#endif