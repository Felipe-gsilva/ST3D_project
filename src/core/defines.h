#pragma once

#define GLFW_INCLUDE_VULKAN
#define REALEASE 0

#ifdef __linux__
#define VK_USE_PLATFORM_XCB_KHR
#endif

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#define u32 uint32_t
#define u64 uint64_t
#define i32 int32_t
#define i64 int64_t

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern const bool enableValidationLayers;

typedef struct Device
{
  VkPhysicalDevice physicalDevice;
  VkPhysicalDeviceProperties properties;
  VkPhysicalDeviceFeatures features;
  VkPhysicalDeviceMemoryProperties memoryProperties;
  VkExtensionProperties *extensions;
} Device;

typedef struct Logger
{
  FILE *file;
  u32 logLevel;
} Logger;

typedef struct App
{
  GLFWwindow *window;
  VkInstance instance;
  VkSurfaceKHR surface;
  Device *device;
  Logger *logger;
} App;

typedef struct QueueFamilyIndices
{
  u32 graphicsFamily;
  u32 presentFamily;
} QueueFamilyIndices;

typedef struct queueCreateInfo
{
  VkDeviceQueueCreateInfo queueCreateInfo;
  float *pQueuePriorities;
} queueCreateInfo;