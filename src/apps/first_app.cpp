#include "first_app.hpp"

#include "game/bor_camera.hpp"
#include "game/keyboard_movement_controller.hpp"

#include "systems/simple_render_system.hpp"
#include "systems/point_light_system.hpp"
#include "systems/ui_system.hpp"

#include "vk/bor_buffer.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <vector>
#include <chrono>


namespace bor
{
    FirstApp::FirstApp()
    {
        globalPool = BoRDescriptorPool::Builder(borDevice)
          .setMaxSets(BoRSwapChain::MAX_FRAMES_IN_FLIGHT)
          .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, BoRSwapChain::MAX_FRAMES_IN_FLIGHT)
          .build();
        loadGameObjects();
    }

    FirstApp::~FirstApp()
    {
    }

    void FirstApp::loadGameObjects()
    {
        std::shared_ptr<BoRModel> borModel = BoRModel::createModelFromFile(borDevice, "C:\\dev\\bowlOfReflections\\models\\flat_vase.obj");
        auto flatVase = BoRGameObject::createGameObject();
        flatVase.model = borModel;
        flatVase.transform.translation = {-0.5f, 0.5f, 0.0f};
        flatVase.transform.scale = {3.0f, 1.5f, 3.0f};
        gameObjects.emplace(flatVase.getId(), std::move(flatVase));

        borModel = BoRModel::createModelFromFile(borDevice, "C:\\dev\\bowlOfReflections\\models\\smooth_vase.obj");
        auto smoothVase = BoRGameObject::createGameObject();
        smoothVase.model = borModel;
        smoothVase.transform.translation = {0.5f, 0.5f, 0.0f};
        smoothVase.transform.scale = {3.0f, 1.5f, 3.0f};
        gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));

        borModel = BoRModel::createModelFromFile(borDevice, "C:\\dev\\bowlOfReflections\\models\\quad.obj");
        auto floor = BoRGameObject::createGameObject();
        floor.model = borModel;
        floor.transform.translation = {0.0f, 0.5f, 0.0f};
        floor.transform.scale = {3.0f, 1.0f, 3.0f};
        gameObjects.emplace(floor.getId(), std::move(floor));

        
        std::vector<glm::vec3> lightColors{
            {1.f, .1f, .1f},
            {.1f, .1f, 1.f},
            {.1f, 1.f, .1f},
            {1.f, 1.f, .1f},
            {.1f, 1.f, 1.f},
            {1.f, 1.f, 1.f}  
        };
        
        for(int i = 0; i < lightColors.size(); i++)
        {
            auto pointLight = BoRGameObject::makePointLight(0.2f);
            pointLight.color = lightColors[i];
            auto rotateLight = glm::rotate(
                glm::mat4(1.0f),
                (i * glm::tau<float>()) / lightColors.size(),
                {0.0f, -1.0f, 0.0f}
            );
            pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f));
            gameObjects.emplace(pointLight.getId(), std::move(pointLight));
        }

    }

    void FirstApp::run()
    {
        std::vector<std::unique_ptr<BoRBuffer>> uboBuffers(BoRSwapChain::MAX_FRAMES_IN_FLIGHT);

        for(int i = 0; i < uboBuffers.size(); ++i)
        {
            uboBuffers[i] = std::make_unique<BoRBuffer>(
                borDevice,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            );

            uboBuffers[i]->map();
        }

         auto globalSetLayout =
            BoRDescriptorSetLayout::Builder(borDevice)
                .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
                .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(BoRSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            BoRDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }


        BoRSimpleRenderSystem simpleRenderSystem{borDevice, borRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
        BoRPointLightSystem pointLightSystem{borDevice, borRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
        BoRUInterfaceSystem uiSystem{borWindow, borDevice, borRenderer.getSwapChainRenderPass()};
        BoRCamera camera{};
        camera.setViewTarget(glm::vec3(-1.0f, -2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 2.5f));
        
        auto viewerObject = BoRGameObject::createGameObject();
        viewerObject.transform.translation.z = -2.5f;
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();
        

        bool showDemoWindow = true;
        while (!borWindow.shouldClose())
        {
            glfwPollEvents();
            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;
            
            cameraController.MoveInPlaneXZ(borWindow.getGLFWWindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = borRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 100.0f);

            if(auto commandBuffer = borRenderer.beginFrame())
            {
                uiSystem.newFrame();
                ImGui::ShowDemoWindow(&showDemoWindow);

                if(glfwGetKey(borWindow.getGLFWWindow(), GLFW_KEY_M) == GLFW_PRESS)
                {
                    std::shared_ptr<BoRModel> borModel = BoRModel::createModelFromFile(borDevice, "C:\\dev\\bowlOfReflections\\models\\flat_vase.obj");
                    auto flatVase = BoRGameObject::createGameObject();
                    flatVase.model = borModel;
                    flatVase.transform.translation = {0.0f, 0.0f, 0.0f};
                    flatVase.transform.scale = {3.0f, 3.5f, 3.0f};
                    gameObjects.emplace(flatVase.getId(), std::move(flatVase));

                }

                // update object in memories
                int frameIndex = borRenderer.getFrameIndex();
                FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex], gameObjects};

                GlobalUbo ubo{};
                ubo.projection = camera.getProjection();
                ubo.view = camera.getView();
                ubo.inverseView = camera.getInverseView();
                pointLightSystem.update(frameInfo, ubo);
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // render
                borRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(frameInfo);
                pointLightSystem.render(frameInfo);
                uiSystem.render(commandBuffer);
                borRenderer.endSwapChainRenderPass(commandBuffer);
                borRenderer.endFrame();
            }

        }   

        vkDeviceWaitIdle(borDevice.device());
    }
}