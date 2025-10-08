#pragma once
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>

#include "VulkanInstance.h"

class VulkanDevice {
    public:
        VulkanDevice(VulkanInstance& instance);
        ~VulkanDevice();

        void pickPhysicalDevice();
        void createDevice();

        std::vector<VkQueue> getQueues() { 
            std::vector<VkQueue> queues{};
            if (m_graphicsQueue != VK_NULL_HANDLE)
                queues.push_back(m_graphicsQueue);
            if (m_presentQueue != VK_NULL_HANDLE)
                queues.push_back(m_presentQueue);
            return queues; 
        }
        VkPhysicalDevice getPhysicalDevice() { return m_physicalDevice; }
        VkDevice getDevice() { return m_device; }
    private:
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkDevice m_device                 = VK_NULL_HANDLE;
        VkQueue m_graphicsQueue           = VK_NULL_HANDLE;
        VkQueue m_presentQueue            = VK_NULL_HANDLE;

        VkInstance m_instance           = VK_NULL_HANDLE; 
};
