#ifndef VK_SIMULATION_DISPLAY
#define VK_SIMULATION_DISPLAY
#include "display.h"
#include "window.h"

#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "lib/display/deligate.h"
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};
struct SwapchainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct UniformBufferObject {
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

class VKSimulationDisplay : public SimulationDisplay {
public:
    VKSimulationDisplay(const WindowCreateInfo windowCreateInfo,
                        std::unique_ptr<DisplaySimulationDeligate> deligate);
    ~VKSimulationDisplay();
    void Update();
    bool isClosed();

private:
    const std::vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation"};
    const std::vector<const char *> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    static VKAPI_ATTR VkBool32 VKAPI_CALL
    debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                  const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                  void *pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage
                  << std::endl;

        return VK_FALSE;
    }

    GLFWwindow *window;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkSurfaceKHR surface;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkSwapchainKHR swapchain;
    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainImageViews;
    VkFormat swapchainImageFormat;
    VkExtent2D swapchainExtent;
    VkRenderPass renderPass;
    VkDescriptorSetLayout descriptorSetLayout;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;
    VkFormat depthFormat;

    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipelineLine;
    VkPipeline graphicsPipelineFill;

    std::vector<VkFramebuffer> swapchainFramebuffers;
    VkCommandPool commandPool;

    std::unique_ptr<DisplaySimulationDeligate> _deligate;

    size_t _numStaticIndices;
    VkBuffer staticVertexBuffer;
    VkDeviceMemory staticVertexBufferMemory;
    VkBuffer staticIndexBuffer;
    VkDeviceMemory staticIndexBufferMemory;

    size_t _numDynamicIndices;
    VkBuffer dynamicVertexBuffer;
    VkDeviceMemory dynamicVertexBufferMemory;
    void *_dynamicVertexMappedMemory;
    VkBuffer dynamicIndexBuffer;
    VkDeviceMemory dynamicIndexBufferMemory;

    VkBuffer uniformBuffer;
    VkDeviceMemory uniformBufferMemory;
    void *uniformBufferMapped;
    const int MAX_FRAMES_IN_FLIGHT = 2;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    bool framebufferResized = false;
    uint32_t currentFrame = 0;

    void initWindow(const WindowCreateInfo windowCreateInfo);
    void initVulkan();

    void createInstance();
    bool checkValidationLayerSupport();
    void setupDebugMessenger();
    void populateDebugMessengerCreateInfo(
        VkDebugUtilsMessengerCreateInfoEXT &createInfo);
    std::vector<const char *> getRequiredExtensions();

    void createSurface();
    void pickPhysicalDevice();
    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device);

    void createLogicalDevice();

    void createSwapchain();
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(
        const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    VkImageView createImageView(VkImage image, VkFormat format,
                                VkImageAspectFlags aspectFlags);
    void createImageViews();

    void createRenderPass();

    void createDescriptorSetLayout();

    void createGraphicsPipelines();
    VkShaderModule createShaderModule(const std::vector<char> &code);

    void createFramebuffers();

    void createCommandPool();

    void createDepthResources();
    VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates,
                                 VkImageTiling tiling,
                                 VkFormatFeatureFlags features);

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                      VkMemoryPropertyFlags properties, VkBuffer &buffer,
                      VkDeviceMemory &bufferMemory);
    uint32_t findMemoryType(uint32_t typeFilter,
                            VkMemoryPropertyFlags properties);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    void createDrawBuffers(DisplaySimulationDeligate &deligate);
    void moveExtractsToStaticDeviceBuffer(const size_t total_size,
                                          const std::vector<Extract> &extracts,
                                          const VkBufferUsageFlags usage,
                                          VkBuffer &outputBuffer,
                                          VkDeviceMemory &outputBufferMemory);
    void *moveExtractsToDynamicDeviceBuffer(
        const size_t total_size, const std::vector<Extract> &extracts,
        const VkBufferUsageFlags usage, VkBuffer &outputBuffer,
        VkDeviceMemory &outputBufferMemory);

    void copyExtractsToMappedMemory(const std::vector<Extract> &extracts,
                                    void *mappedMemory);

    void createUniformBuffer();

    void createDescriptorPool();

    void createDescriptorSets();

    void createCommandBuffers();

    void createSyncObjects();

    void drawFrame();
    void recreateSwapchain();
    void cleanupSwapchain();
    void recordCommandBuffer(VkCommandBuffer commandBuffer,
                             uint32_t imageIndex);
};
#endif