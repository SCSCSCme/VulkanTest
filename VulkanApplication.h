#pragma once
#include "VulkanInstance.h"
#include "VulkanDevice.h"

#include <GLFW/glfw3.h>
#include <memory>

class VulkanApplication {
private:
    GLFWwindow* window;
    std::unique_ptr<VulkanInstance> instance;  // 使用智能指针
    std::unique_ptr<VulkanDevice> device;

public:
    VulkanApplication(const char* appName);
    ~VulkanApplication();
    void run();

    // 禁用拷贝
    VulkanApplication(const VulkanApplication&) = delete;
    VulkanApplication& operator=(const VulkanApplication&) = delete;
};
