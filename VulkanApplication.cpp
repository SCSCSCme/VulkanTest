#include "VulkanApplication.h"
#include <stdexcept>

VulkanApplication::VulkanApplication(const char* appName) : m_window(nullptr) {
    std::cout << "-- Application: Start initilaze. \n";
    if (!glfwInit()) {
        throw std::runtime_error("-- Application: GLFW initialization failed");
    }
    
    std::cout << "-- Application: Setup envroiment. \n";
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    std::cout << "-- Application: Create window. \n";
    m_window = glfwCreateWindow(800, 600, appName, nullptr, nullptr);
    if (!m_window) {
        glfwTerminate();
        throw std::runtime_error("-- Application: Window creation failed");
    }
    
    // 使用make_unique创建实例，避免拷贝问题
    std::cout << "-- Application: Create instance. \n";
    m_instance = std::make_unique<VulkanInstance>(appName, m_window);
    std::cout << "-- Application: Create device. \n";
    m_device = std::make_unique<VulkanDevice>(*m_instance);
    std::cout << "-- Application: Create Swapchain. \n";
    m_swapchain = std::make_unique<VulkanSwapchain>(*m_device);

}

VulkanApplication::~VulkanApplication() {
    // 智能指针会自动管理内存
    std::cout << "-- Application: Destroy. \n";
    if (m_window) {
        std::cout << "-- Application: Destroy window. \n";
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    std::cout << "-- Application: Free last one. \n";
    glfwTerminate();
}

void VulkanApplication::run() {
    std::cout << "-- Application: Start mainloop. \n";
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
    }
    std::cout << "-- Application: Exit mainloop. \n";
}
