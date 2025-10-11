#include "VulkanSwapchain.h"

VulkanSwapchain::VulkanSwapchain(VulkanDevice& device) {
    std::cout << "-- Swapchain: Create. \n";

    m_pysDevice = device.getPhysicalDevice();
    m_device = device.getDevice();
    m_queues = device.getQueues();
    create();
}

void VulkanSwapchain::create() {
    std::cout << "-- Swapchain: Create swapchain. \n";
    VkSwapchainCreateInfoKHR swapchainCreateInfo{};
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.pNext = nullptr;
    swapchainCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    swapchainCreateInfo.format = VKVK_FORMAT_RBG888;
    swapchainCreateInfo.imageColorSpace = VK_COLOR_SRGB_SPACE;
}

void VulkanSwapchain::recreate() {
    cleanup();
    create();
}

void VulkanSwapchain::cleanup() {

}

VulkanSwapchain::~VulkanSwapchain() {
    std::cout << "-- Swapchain: Destroy. \n";
    cleanup();
}
