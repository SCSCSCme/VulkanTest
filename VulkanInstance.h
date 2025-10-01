#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
#include <iostream>

class VulkanInstance {
private:
    VkInstance instance;
    GLFWwindow* window;
    VkSurfaceKHR surface;

public:
    // 正确定义构造函数
    VulkanInstance(const char* appName, GLFWwindow* win);
    ~VulkanInstance();

    VkInstance getInstance();
    VkSurfaceKHR getSurface();

    // 删除拷贝构造函数和赋值运算符
    VulkanInstance(const VulkanInstance&) = delete;
    VulkanInstance& operator=(const VulkanInstance&) = delete;

private:
    void createInstance(const char* appName);
    void createSurface();
};
