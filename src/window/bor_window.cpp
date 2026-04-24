#include "bor_window.hpp"
#include <stdexcept>

namespace bor
{
    BoRWindow::BoRWindow(int w, int h, std::string name)
        : width{w}, height{h}, windowName{name}
    {
        initWindow();
    }

    BoRWindow::~BoRWindow()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void BoRWindow::initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    void BoRWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
    {
        if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS){
            throw std::runtime_error("failed to create window surface");
        }
    }

    void BoRWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
    {
        auto borWindow = reinterpret_cast<BoRWindow*>(glfwGetWindowUserPointer(window));
        borWindow->framebufferResized = true;
        borWindow->width = width;
        borWindow->height = height;
    }
}