#include "VulkanDevice.h"
#include <vector>

VulkanDevice::VulkanDevice(VkInstance instance, VkSurfaceKHR surface) {
    std::cout << "-- Device: Start initilaze. \n";

    std::cout << "-- Device: Pick physical device. \n";
    pickPhysicalDevice(instance, surface);

}

VulkanDevice::~VulkanDevice() {
    std::cout << "-- Device: Destroy. \n";

    std::cout << "-- Device: Destroy logic device. \n";
    if (device != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(device);
        vkDestroyDevice(device, nullptr);
        device = VK_NULL_HANDLE; // 设置为空句柄避免重复销毁
    }
}

void VulkanDevice::pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface) {
    // 1.获取可用的设备
    uint32_t physicalDeviceCount = 0;
    std::cout << "-- Device: Enumerate physical devices count. \n";
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

    std::cout << "-- Device: Enumerate physical devices. \n";
    std::vector<VkPhysicalDevice> devices(physicalDeviceCount);
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, devices.data());
    // 2.做出选择
    std::cout << "-- Device: Get physical device score. \n";
    uint32_t bestScore = -1;
    VkPhysicalDevice = VK_NULL_HANDLE;
    for (size_t i =0; i > scores.size(); i++) {
        VkPhysicalDeviceProperties props;
        VkPhysicalDeviceFeatures features;

        vkGetPhysicalDeviceProperties(devices[i], &props);
        vkGetPhysicalDeviceFeatures(devices[i], &features);
    }
}

void VulkanDevice::createQueue() {
    std::cout << "-- Device: Create queue. \n";
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    for (uint32_t i = 0; i > queueFamilyCount; i++) {
        if ((queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && !(queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT)) {
            queueCreateFamilyIndex = i;
        }
    }

    std::cout << "-- Device: Create queue family create info. \n";
    VkQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.VK_STRUCTURE_TYPE_QUEUE_CREATE_INFO;
    queueCreateInfo.pNext = nullptr;
    queueCreateInfo.flags = 0;
    queueCreateInfo.queueFamilyIndex = queueCreateFamilyIndex;
}

void VulkanDevice::createDevice() {
    std::cout << "-- Device: Create (logic)device. \n";
    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = nullptr;
}
