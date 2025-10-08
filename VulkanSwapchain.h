#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "VulkanDevice.h"

class VulkanSwapchain {
    public:
        VulkanSwapchain(VulkanDevice& device);
        ~VulkanSwapchain();

        void create();
        void recreate();
        void cleanup();
    private:
        VkSwapchainKHR m_swapchain    = VK_NULL_HANDLE;
        std::vector<VkImage> m_images {};

        VkPhysicalDevice m_pysDevice  = VK_NULL_HANDLE;
        VkDevice m_device             = VK_NULL_HANDLE;
        std::vector<VkQueue> m_queues {};
};
