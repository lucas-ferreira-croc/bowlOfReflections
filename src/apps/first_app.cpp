#include "first_app.hpp"

#include "game/bor_camera.hpp"
#include "game/keyboard_movement_controller.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <chrono>

namespace bor
{
    FirstApp::FirstApp()
    {
        loadGameObjects();
    }

    FirstApp::~FirstApp()
    {
    }

    std::unique_ptr<BoRModel> createCubeModel(BoRDevice& device, glm::vec3 offset) {
        BoRModel::Builder modelBuilder{};

         modelBuilder.vertices = {
            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
        
            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
        
            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        
            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        
            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        
            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        
        };

        for (auto& v : modelBuilder.vertices) {
            v.position += offset;
        }

         modelBuilder.indices = {0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
                          12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};
        
        return std::make_unique<BoRModel>(device, modelBuilder);
    }

    void FirstApp::loadGameObjects()
    {
        bool useSierpinski = false;

        std::shared_ptr<BoRModel> cubeModel = createCubeModel(borDevice, glm::vec3(0.0f));

        auto cube = BoRGameObject::createGameObject();
        cube.model = cubeModel;
        cube.transform.translation = {0.0f, 0.0f, 2.5f};
        cube.transform.scale = {0.5f, 0.5f, 0.5f};
        gameObjects.push_back(std::move(cube));
    }

    void FirstApp::run()
    {
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
                borRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
                borRenderer.endSwapChainRenderPass(commandBuffer);
                borRenderer.endFrame();
            }

        }   

        vkDeviceWaitIdle(borDevice.device());
    }
}