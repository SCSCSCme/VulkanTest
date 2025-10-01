#pragma once
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>

class VulkanDevice {
    public:
        VulkanDevice(VkInstance instance, VkSurfaceKHR surface);
        ~VulkanDevice();

        void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
        void createQueue();
        void createDevice();
       
    private:
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device                 = VK_NULL_HANDLE;
        VkQueue graphicsQueue           = VK_NULL_HANDLE;
        VkQueue presentQueue            = VK_NULL_HANDLE;
};
