#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

#include <bitset>

// Flags:
// Physical Device Type flags:
// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPhysicalDeviceType.html
// Queue flags:
// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkQueueFlagBits.html
// Memory properties:
// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkMemoryPropertyFlagBits.html
// Heap flags:
// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkMemoryHeapFlagBits.html
//

int main(int argc, char **argv) {
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = 0;
    createInfo.enabledLayerCount = 0;

    VkInstance instance;
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    VkPhysicalDeviceProperties properties;
    for (const auto &device : devices) {

        // Check general properties
        vkGetPhysicalDeviceProperties(device, &properties);

        std::bitset<8> pdt(properties.deviceType);
        std::cout << properties.deviceName << std::endl;
        std::cout << "Device Type: " << pdt << std::endl;
        // std::cout << "Max Vertex Input Bindings: "
        //    << properties.limits.maxVertexInputBindings << std::endl;

        // Check Queue Family Properties
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                                 nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                                 queueFamilies.data());

        std::cout << "  Queue Families:" << std::endl;
        int i = 0;
        for (const auto &queueFamily : queueFamilies) {

            std::bitset<8> qfl(queueFamily.queueFlags);
            std::cout << "    Queue Family #" << i << std::endl
                      << "    Queue Count: " << queueFamily.queueCount
                      << std::endl
                      << "    Queue Flags: " << qfl << std::endl;
            i++;
        }

        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(device, &memProperties);

        std::cout << "  Memory Properties:" << std::endl;
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            std::bitset<9> mtpf(memProperties.memoryTypes[i].propertyFlags);
            std::cout << "    Memory Type #" << i << std::endl;
            std::cout << "      Property flags: " << mtpf << std::endl;
            std::cout << "      Heap index: "
                      << memProperties.memoryTypes[i].heapIndex << std::endl;
        }

        for (uint32_t i = 0; i < memProperties.memoryHeapCount; i++) {
            std::cout << "    Memory Heap #" << i << std::endl;
            std::bitset<2> mhf(memProperties.memoryHeaps[i].flags);
            std::cout << "      Flags: " << mhf << std::endl;
            std::cout << "      Size: " << memProperties.memoryHeaps[i].size
                      << std::endl;
        }
    }

    vkDestroyInstance(instance, nullptr);
}
