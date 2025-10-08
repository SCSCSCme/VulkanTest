#include "VulkanSwapchain.h"

VulkanSwapchain::VulkanSwapchain(VulkanDevice& device) {
    std::cout << "-- Swapchain: Create. \n";

    m_pysDevice = device.getPhysicalDevice();
    m_device = device.getDevice();
    m_queues = device.getQueues();
    create();
}

void VulkanSwapchain::create() {

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
