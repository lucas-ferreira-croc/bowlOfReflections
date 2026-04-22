#ifndef BOR_WINDOW_HPP
#define BOR_WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace bor
{
    class BoRWindow
    {
    public:
        BoRWindow(int w, int h, std::string name);
        ~BoRWindow();

        BoRWindow(const BoRWindow&) = delete;
        BoRWindow &operator=(const BoRWindow&) = delete;

        bool shouldClose() { return glfwWindowShouldClose(window); }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
    private:
        
        void initWindow();

        int width;
        int height;
        std::string windowName;
        GLFWwindow* window;
    };
}

#endif