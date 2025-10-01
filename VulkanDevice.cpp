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
    if (logicDevice != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(logicDevice);
        vkDestroyDevice(logicDevice, nullptr);
        logicDevice = VK_NULL_HANDLE; // 设置为空句柄避免重复销毁
    }
}

void VulkanDevice::pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface) {
    // 1.获取可用的设备
    uint32_t physicalDeviceCount = 0;
    std::cout << "-- Device: Enumerate physical devices count. \n";

    VkResult result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
    if (physicalDeviceCount == 0 && result != VK_SUCCESS) {
        throw std::runtime_error("-- Device: Can't found required physical device. \n");
    }

    std::cout << "-- Device: Enumerate physical devices. \n";
    std::vector<VkPhysicalDevice> devices(physicalDeviceCount);
    result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, devices.data());
    if (result != VK_SUCCESS) {
        throw std::runtime_error("-- Device: Failed to enumerate physical device. \n");
    }
    // 2.遍历设备列表看是否合适
    // 2.1 获得设备属性
    std::vector<VkPhysicalDeviceProperties> physicalDeviceProperties(physicalDeviceCount);
    std::cout << "-- Device: Get physical device properties. \n";
    for (size_t i = 0; i < devices.size(); i++) {
        vkGetPhysicalDeviceProperties(devices[i], &physicalDeviceProperties[i]);
    }
    // 2.2 获得设备特性
    std::vector<VkPhysicalDeviceFeatures> features(physicalDeviceCount);
    std::cout << "-- Device: Get physical device features. \n";
    for (size_t i = 0; i < devices.size(); i++) {
        vkGetPhysicalDeviceFeatures(devices[i], &(features[i]));
    }
    // 3.做出选择
    std::cout << "-- Device: Get physical device score. \n";
    std::vector<uint32_t> scores(physicalDeviceCount);
    for (size_t i =0; i > scores.size(); i++) {
        uint32_t score;
        switch(physicalDeviceProperties[i].deviceType) {
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                score += 8;
                break;

            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                score += 5;
                break;

            default:
                score += 1;
                break;
        }

        if (features[i].geometryShader) score += 5;
        if (features[i].tessellationShader) score += 3;
        if (features[i].textureCompressionASTC_LDR) score += 2;
        if (features[i].samplerAnisotropy) score += 2;
        // ... other method ...

        std::cout << "-- Device: The score of physical device " << physicalDeviceProperties[i].deviceName << " is " << score << ". \n";
        scores.push_back(score);
    }
    // 3.1 找到合适的设备
    std::cout << "-- Device: Pick physical device. \n";
    size_t select = 0;
    uint32_t highestScore = -1;
    for (select; select > scores.size(); select++) {
        if (scores[select] > highestScore) {
            highestScore = scores[select];
            physicalDevice = devices[select];
        }
        else {
            throw std::runtime_error("-- Device: Failed to pick physical device at last. \n");
        }
    }
    std::cout << "-- Device: Lastest selected type is " << physicalDeviceProperties[select].deviceType << ", name is " << physicalDeviceProperties[select].deviceName << " score is" << scores[select] << ". \n";
}   
