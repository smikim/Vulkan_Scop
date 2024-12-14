#include "Scop.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

int main() {
    glfwInit();

    scop::Scop * scop;
    try
    {
        scop = new scop::Scop();

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        scop->run();
    }
    catch (const std::bad_alloc& e)
    {
        std::cerr << "malloc error: " << e.what() << std::endl;
        return false;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    delete scop;

    return EXIT_SUCCESS;

    return 0;
}