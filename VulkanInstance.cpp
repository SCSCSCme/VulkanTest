#include "VulkanInstance.h"
#include <iostream>

VulkanInstance::VulkanInstance(const char* appName, GLFWwindow* win) 
    : window(win), instance(VK_NULL_HANDLE), surface(VK_NULL_HANDLE) {
        std::cout << "-- Instance: Start create. \n";
        
        std::cout << "-- Instance: Create instance. \n";
        createInstance(appName);
        std::cout << "-- Instance: Create surface. \n";
        createSurface();
}

VulkanInstance::~VulkanInstance() {
    std::cout << "-- Instance: Start destroy. \n";
    if (surface != VK_NULL_HANDLE) {
        std::cout << "-- Instance: Destroy surface. \n";
        vkDestroySurfaceKHR(instance, surface, nullptr);
    }
    if (instance != VK_NULL_HANDLE) {
        std::cout << "-- Instance: Destory instance. \n";
        vkDestroyInstance(instance, nullptr);
    }
}

void VulkanInstance::createInstance(const char* appName) {
    std::cout << "-- Instance: Create appInfo. \n";
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = appName;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // 获取GLFW需要的扩展
    std::cout << "-- Instance: Get required extensions. \n";
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::cout << "-- Instance: Create createInfo. \n";
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    std::cout << "-- Instance: Create instance. \n";
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("--Instance: Failed to create Vulkan instance!\n");
    }

    std::cout << "-- Instance: Check available extensions. \n";
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::cout << "-- Instance: \nAvailable Vulkan extensions:\n";
    for (const auto& extension : extensions) {
        std::cout << "\t" << extension.extensionName << "\n";
    }
}

void VulkanInstance::createSurface() {
    VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("-- Instance: Failed to create window surface!\n");
    }
}

VkInstance VulkanInstance::getInstance() { return instance; }
VkSurfaceKHR VulkanInstance::getSurface() { return surface; }
