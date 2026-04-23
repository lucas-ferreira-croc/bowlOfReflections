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
        VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
    private:
        
        void initWindow();

        const int width;
        const int height;
        std::string windowName;
        GLFWwindow* window;
    };
}

#endif