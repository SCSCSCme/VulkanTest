#include "VulkanApplication.h"
#include <iostream>
#include <stdexcept>

int main() {
    std::cout << "-- Main: Program run. \n";

    try {
        // 正确创建VulkanApplication，不是VulkanInstance
        std::cout << "-- Main: Create VulkanApplication. \n";
        VulkanApplication app("Vulkan Test");
        std::cout << "-- Main: Run VulkanApplication. \n";
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    std::cout << "-- Main: Program exit. \n";
    return EXIT_SUCCESS;
}
