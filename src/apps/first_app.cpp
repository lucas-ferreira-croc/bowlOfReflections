#include "first_app.hpp"

#include "game/bor_camera.hpp"
#include "game/keyboard_movement_controller.hpp"

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
    struct GlobalUbo
    {
        glm::mat4 projectionView{1.0f};
        glm::vec3 lightDirection = glm::normalize(glm::vec3{1.0f, -3.0f, -1.0f});
    };

    FirstApp::FirstApp()
    {
        loadGameObjects();
    }

    FirstApp::~FirstApp()
    {
    }

    void FirstApp::loadGameObjects()
    {
        bool useSierpinski = false;

        std::shared_ptr<BoRModel> borModel = BoRModel::createModelFromFile(borDevice, "C:\\dev\\bowlOfReflections\\models\\teapot.obj");

        auto gameObject = BoRGameObject::createGameObject();
        gameObject.model = borModel;
        gameObject.transform.translation = {0.0f, 0.5f, 2.5f};
        gameObject.transform.scale = glm::vec3(1.0f);
        gameObjects.push_back(std::move(gameObject));
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

        BoRSimpleRenderSystem simpleRenderSystem{borDevice, borRenderer.getSwapChainRenderPass()};
        BoRCamera camera{};
        camera.setViewTarget(glm::vec3(-1.0f, -2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 2.5f));
        
        auto viewerObject = BoRGameObject::createGameObject();
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();
        while (!borWindow.shouldClose())
        {
            glfwPollEvents();
            
            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;
            
            cameraController.MoveInPlaneXZ(borWindow.getGLFWWindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = borRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 10.0f);

            if(auto commandBuffer = borRenderer.beginFrame())
            {
                // update object in memories
                int frameIndex = borRenderer.getFrameIndex();
                GlobalUbo ubo{};
                ubo.projectionView = camera.getProjection() * camera.getView();
                uboBuffers[frameIndex]->writeToBuffer(&ubo, frameIndex);
                uboBuffers[frameIndex]->flush();

                FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera};
                // render
                borRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
                borRenderer.endSwapChainRenderPass(commandBuffer);
                borRenderer.endFrame();
            }

        }   

        vkDeviceWaitIdle(borDevice.device());
    }
}