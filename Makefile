makefile:

.PHONY: build run

build:
	C:\VulkanSDK\1.4.341.1\Bin\glslc.exe shaders\simple_shader.vert -o  shaders\simple_shader.vert.spv && \
	C:\VulkanSDK\1.4.341.1\Bin\glslc.exe shaders\simple_shader.frag -o  shaders\simple_shader.frag.spv && \
	cmake -B build && \
	cmake --build build
run:
	C:\dev\bowlOfReflections\bin\Debug\Windows\bowl_of_reflections.exe
