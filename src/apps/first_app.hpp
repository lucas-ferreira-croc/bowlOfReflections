#ifndef FIRST_APP_HPP
#define FIRST_APP_HPP

#include "window/bor_window.hpp"
#include "vk/bor_pipeline.hpp"
#include "vk/bor_device.hpp"

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
        BoRDevice borDevice{borWindow};
        BoRPipeline borPipeline{borDevice, "C:\\dev\\bowlOfReflections\\shaders\\simple_shader.vert.spv", "C:\\dev\\bowlOfReflections\\shaders\\simple_shader.frag.spv", BoRPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
    };
}

#endif