#ifndef FIRST_APP_HPP
#define FIRST_APP_HPP

#include "window/bor_window.hpp"

namespace bor
{
    class FirstApp
    {
    public:
        static constexpr int WIDTH = 2048;
        static constexpr int HEIGHT = 1536;

        void run();

    private:
        BoRWindow borWindow{WIDTH, HEIGHT, "Bowl of Reflections"};
    };
}

#endif