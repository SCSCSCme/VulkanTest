#include "VulkanApplication.h"
#include <stdexcept>

VulkanApplication::VulkanApplication(const char* appName) : window(nullptr) {
    std::cout << "-- Application: Start initilaze. \n";
    if (!glfwInit()) {
        throw std::runtime_error("-- Application: GLFW initialization failed");
    }
    
    std::cout << "-- Application: Setup envroiment. \n";
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    std::cout << "-- Application: Create window. \n";
    window = glfwCreateWindow(800, 600, appName, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("-- Application: Window creation failed");
    }
    
    // 使用make_unique创建实例，避免拷贝问题
    std::cout << "-- Application: Create instance. \n";
    instance = std::make_unique<VulkanInstance>(appName, window);
    std::cout << "-- Application: Create device. \n";
    device = std::make_unique<VulkanDevice>(instance->getInstance(), instance->getSurface());

}

VulkanApplication::~VulkanApplication() {
    // 智能指针会自动管理内存
    std::cout << "-- Application: Start destroy. \n";
    if (window) {
        std::cout << "-- Application: Destroy window. \n";
        glfwDestroyWindow(window);
        window = nullptr;
    }
    std::cout << "-- Application: Free last one. \n";
    glfwTerminate();
}

void VulkanApplication::run() {
    std::cout << "-- Application: Start mainloop. \n";
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
    std::cout << "-- Application: Exit mainloop. \n";
}
