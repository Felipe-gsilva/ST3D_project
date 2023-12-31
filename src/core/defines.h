#pragma once

#define GLFW_INCLUDE_VULKAN
#define REALEASE 0

#ifdef __linux__
#define VK_USE_PLATFORM_XCB_KHR
#define LINUX 1
#endif

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_EXPOSE_NATIVE_WIN32
#define WINDOWS 1
#endif

#define u32 uint32_t
#define u64 uint64_t
#define i32 int32_t
#define i64 int64_t

#define UINT_MAX 800

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern const bool enableValidationLayers;
extern const char *validationLayers[];
extern const u32 validationLayerCount;

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData);

typedef struct ShaderFile
{
  size_t size;
  char *code;
} ShaderFile;

typedef struct Device
{
  VkPhysicalDevice physicalDevice;
  VkPhysicalDeviceProperties properties;
  VkPhysicalDeviceFeatures features;
  VkExtensionProperties *extensions;
  VkDevice logicalDevice;
  VkQueue graphicsQueue;
  VkQueue presentQueue;
} Device;

typedef struct Logger
{
  FILE *file;
  u32 logLevel;
} Logger;

typedef struct QueueFamilyIndices
{
  u32 graphicsFamily;
  bool isGraphicsFamilySet;
  u32 presentFamily;
  bool isPresentFamilySet;
} QueueFamilyIndices;

typedef struct queueCreateInfo
{
  VkDeviceQueueCreateInfo queueCreateInfo;
  float *pQueuePriorities;
} queueCreateInfo;

typedef struct SwapChainSupportDetails
{
  VkSurfaceCapabilitiesKHR capabilities;
  VkSurfaceFormatKHR *formats;
  u32 formatCount;
  VkPresentModeKHR *presentModes;
  u32 presentModeCount;
  VkExtent2D *extent;
} SwapChainSupportDetails;

typedef struct App
{
  GLFWwindow *window;
  VkInstance instance;
  VkDebugUtilsMessengerEXT debugMessenger;
  VkSurfaceKHR surface;
  VkPhysicalDevice *deviceList;
  Device *device;
  Logger *logger;
  SwapChainSupportDetails details;
  VkSwapchainKHR swapChain;
  u32 swapChainImageCount;
  VkImage *swapChainImages;
  VkImageView *swapChainImageViews;
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;
  u32 shaderCount;
  VkPipelineShaderStageCreateInfo *shaderStages;
  VkRenderPass renderPass;
  VkPipelineLayout pipelineLayout;
  VkPipeline graphicsPipeline;
  VkFramebuffer *swapChainFramebuffers;
  VkCommandPool commandPool;
  VkCommandBuffer commandBuffer;
  u32 imageIndex;
  VkSemaphore imageAvailableSemaphore;
  VkSemaphore renderFinishedSemaphore;
  VkFence inFlightFence;
} App;