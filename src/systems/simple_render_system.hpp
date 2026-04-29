#ifndef BOR_SIMPLE_RENDER_SYSTEM_HPP
#define BOR_SIMPLE_RENDER_SYSTEM_HPP

#include "game/bor_camera.hpp"
#include "vk/bor_pipeline.hpp"
#include "vk/bor_device.hpp"

#include "game/bor_game_object.hpp"

#include <memory>
#include <vector>

namespace bor
{
    class BoRSimpleRenderSystem
    {
    public:
        BoRSimpleRenderSystem() = default;
        BoRSimpleRenderSystem(BoRDevice& device, VkRenderPass renderPass);
        ~BoRSimpleRenderSystem();

        BoRSimpleRenderSystem(const BoRSimpleRenderSystem&) = delete;
        BoRSimpleRenderSystem &operator=(const BoRSimpleRenderSystem&) = delete;

        void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<BoRGameObject>& gameObjects, const BoRCamera& camera);
    private:
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

        BoRDevice& borDevice;
          
        std::unique_ptr<BoRPipeline> borPipeline;
        VkPipelineLayout pipelineLayout;


    };
}

#endif