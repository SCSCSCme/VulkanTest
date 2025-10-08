#pragma once
#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"

#include <GLFW/glfw3.h>
#include <memory>

class VulkanApplication {
private:
    GLFWwindow* m_window;
    std::unique_ptr<VulkanInstance> m_instance;  // 使用智能指针
    std::unique_ptr<VulkanDevice> m_device;
    std::unique_ptr<VulkanSwapchain> m_swapchain;

public:
    VulkanApplication(const char* appName);
    ~VulkanApplication();
    void run();

    // 禁用拷贝
    VulkanApplication(const VulkanApplication&) = delete;
    VulkanApplication& operator=(const VulkanApplication&) = delete;
};
