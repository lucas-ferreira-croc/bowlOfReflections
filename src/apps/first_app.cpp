#include "first_app.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>

namespace bor
{
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

        std::vector<BoRModel::Vertex> vertices;

        std::function<void(int, glm::vec2, glm::vec2, glm::vec2)> sierpinski;
        sierpinski = [&](int depth, glm::vec2 left, glm::vec2 right, glm::vec2 top)
        {
            if (depth <= 0)
            {
                vertices.push_back({top, {1.0f, 0.0f, 0.0f}});
                vertices.push_back({right, {0.0f, 1.0f, 0.0f}});
                vertices.push_back({left, {0.0f, 0.0f, 1.0f}});
            }
            else
            {
                auto leftTop = 0.5f * (left + top);
                auto rightTop = 0.5f * (right + top);
                auto leftRight = 0.5f * (left + right);

                sierpinski(depth - 1, left, leftRight, leftTop);
                sierpinski(depth - 1, leftRight, right, rightTop);
                sierpinski(depth - 1, leftTop, rightTop, top);
            }
        };
        
        if(useSierpinski)
        {
            sierpinski(15, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});
        }
        else 
        {
            vertices = {
                {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                {{0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
                {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
            };
        }
        auto borModel = std::make_shared<BoRModel>(borDevice, vertices);

        auto triangle = BoRGameObject::createGameObject();
        triangle.model = borModel;
        triangle.color = {.4, 0.0, .7f };
        triangle.transform2D.translation.x = .2f;
        triangle.transform2D.scale = {2.0f, 0.5f};
        triangle.transform2D.rotation = 0.25f * glm::two_pi<float>();

        gameObjects.push_back(std::move(triangle));
    }

    void FirstApp::run()
    {
        BoRSimpleRenderSystem simpleRenderSystem{borDevice, borRenderer.getSwapChainRenderPass()};

        while (!borWindow.shouldClose())
        {
            glfwPollEvents();
            
            if(auto commandBuffer = borRenderer.beginFrame())
            {
                borRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
                borRenderer.endSwapChainRenderPass(commandBuffer);
                borRenderer.endFrame();
            }

        }   

        vkDeviceWaitIdle(borDevice.device());
    }
}