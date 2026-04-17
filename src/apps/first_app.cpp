#include "first_app.hpp"

namespace bor
{
    void FirstApp::run()
    {
        while (!borWindow.shouldClose())
        {
            glfwPollEvents();
        }
        
    }
}