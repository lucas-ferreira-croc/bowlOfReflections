#include "bor_window.hpp"

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
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    }
}