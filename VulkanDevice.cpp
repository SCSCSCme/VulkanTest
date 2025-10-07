#include "VulkanDevice.h"
#include <vector>
#include <set>
#include <algorithm>
#include <string>

VulkanDevice::VulkanDevice(VkInstance instance, VkSurfaceKHR surface) {
    std::cout << "-- Device: Start initilaze. \n";

    std::cout << "-- Device: Pick physical device. \n";
    pickPhysicalDevice(instance, surface);
    if (physicalDevice == VK_NULL_HANDLE) {
        std::cout << "-- Device: [ERROR]->physicalDevice is null after pickPhysicalDevice! \n";
    } else {
        std::cout << "-- Device: [SUCCESS]->physicalDevice is valid after pickPhysicalDevice. \n";
    }
    std::cout << "-- Device: Create (logic) device. \n";
    createDevice();
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
    if (instance == VK_NULL_HANDLE) {
        throw std::runtime_error("-- Device: The instace is null. Exit failed. \n");
    } else if (surface == VK_NULL_HANDLE) {
        throw std::runtime_error("-- Device: The surface is null. Exit failed. \n");
    }
    // 1.获取可用的设备
    uint32_t physicalDeviceCount = 0;
    std::cout << "-- Device: Enumerate physical devices count. \n";
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

    std::cout << "-- Device: Enumerate physical devices. \n";
    std::vector<VkPhysicalDevice> devices(physicalDeviceCount);
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, devices.data());
    // 2.做出选择
    std::cout << "-- Device: Get physical device score. \n";
    int32_t bestScore = -1;
    VkPhysicalDevice bestPhysicalDevice = VK_NULL_HANDLE;

    size_t select = 0;
    for (; select < physicalDeviceCount; select++) {
        int32_t score = 0;
        VkPhysicalDeviceProperties props;
        VkPhysicalDeviceFeatures features;

        vkGetPhysicalDeviceProperties(devices[select], &props);
        vkGetPhysicalDeviceFeatures(devices[select], &features);

        uint32_t queueFamilyCount;
        vkGetPhysicalDeviceQueueFamilyProperties(devices[select], &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(devices[select], &queueFamilyCount, queueFamilies.data());

        bool hasGraphicsQueue = false;
        for (const auto& q : queueFamilies) {
            if (q.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                hasGraphicsQueue = true;
                break;
            }
        }
        if (!hasGraphicsQueue) {
            std::cout << "-- Device: [REJECTED]->" << props.deviceName << " - No graphics queue. \n";
            continue;
        }

        std::set<std::string> requiredExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(devices[select], nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(devices[select], nullptr, &extensionCount, extensions.data());

        for (const auto& ext : extensions) {
            requiredExtensions.erase(ext.extensionName);
        }
        if (!requiredExtensions.empty()) { 
            std::cout << "-- Device: [REJECTED]->" << props.deviceName << " - No swapchain. \n";
            continue;
        }

        // --- 3. 优先排除 CPU（llvmpipe）---
        if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU) {
            std::cout << "-- Device: [REJECTED]->" << props.deviceName << " is CPU (llvmpipe). Skipping.\n";
            continue;
        }

        switch (props.deviceType) {
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                score += 800;
                break;
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                score += 500;
                break;
            case VK_PHYSICAL_DEVICE_TYPE_CPU:
                score += 10;
                break;
            default:
                score += 10;
                break;
        }
        uint32_t apiVersion = props.apiVersion;
        uint32_t major = VK_VERSION_MAJOR(apiVersion);
        uint32_t minor = VK_VERSION_MINOR(apiVersion);

        if (major > 1 || (major == 1 && minor >= 3)) {
            score += 200;
        } else if (major == 1 && minor >= 2) {
            score += 100;
        } else if (major == 1 && minor >= 1) {
            score += 50;
        } else {
            score += 10; // 1.0 或更低（不推荐）
        }
        // --- 6. 可选特性加分 ---
        if (features.geometryShader) score += 50;
        if (features.samplerAnisotropy) score += 30;
        if (features.textureCompressionBC) score += 30;

        // --- 7. 设备本地显存加分（限制最大 500）---
        VkPhysicalDeviceMemoryProperties memProps;
        vkGetPhysicalDeviceMemoryProperties(devices[select], &memProps);
        for (uint32_t i = 0; i < memProps.memoryHeapCount; ++i) {
            if (memProps.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
                uint64_t sizeMB = memProps.memoryHeaps[i].size / (1024 * 1024);
                score += std::min(static_cast<uint64_t>(500), sizeMB);
                break;
            }
        }
        if (bestScore < score) { 
            bestScore = score;
            bestPhysicalDevice = devices[select];
        }
    }
    if (bestPhysicalDevice == VK_NULL_HANDLE) {
        std::cout << "-- Device: [ERROR]->No device passed all checks!\n";
    }
    this->physicalDevice = bestPhysicalDevice;
}

void VulkanDevice::createDevice() {
    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("-- Device: The physical device is null. Exit failed. \n");
    }
    // 1. 获取图形队列族索引
    int32_t queueFamilyIndex = -1;
    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    for (uint32_t i = 0; i < queueFamilyCount; ++i) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queueFamilyIndex = i;
            break;
        }
    }

    if (queueFamilyIndex == -1) {
        throw std::runtime_error("-- Device: No graphics queue family found!");
    }

    // 2. 创建队列创建信息
    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    // 3. 启用设备特性（可选）
    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy = VK_TRUE; // 推荐启用

    // 4. 启用扩展（必须包含交换链！）
    const char* deviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    uint32_t extensionCount = 1;

    // 5. 构建设备创建信息
    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = nullptr;
    deviceCreateInfo.queueCreateInfoCount = 1;           // ✅ 必须设置！
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo; // ✅ 必须设置！
    deviceCreateInfo.enabledExtensionCount = extensionCount; // ✅ 必须设置！
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions; // ✅ 必须设置！
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;   // ✅ 必须设置！

    // 6. 创建逻辑设备
    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("-- Device: Failed to create logical device!");
    }

    std::cout << "-- Device: Logical device created successfully.\n";

    // 7. 获取图形队列
    vkGetDeviceQueue(device, queueFamilyIndex, 0, &graphicsQueue);
    std::cout << "-- Device: Graphics queue obtained.\n";
}
