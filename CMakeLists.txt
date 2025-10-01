cmake_minimum_required(VERSION 3.12)
project(VulkanTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(SandBox SandBox.cpp VulkanApplication.cpp  VulkanInstance.cpp VulkanDevice.cpp)
target_link_libraries(SandBox PRIVATE Vulkan::Vulkan glfw)
target_link_options(SandBox PRIVATE "/NODEFAULTLIB:MSVCRT")
