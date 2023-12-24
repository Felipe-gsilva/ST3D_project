#pragma once

#define GLFW_INCLUDE_VULKAN
#define REALEASE 0
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_EXPOSE_NATIVE_WIN32

#define u32 uint32_t
#define u64 uint64_t
#define i32 int32_t
#define i64 int64_t

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

char *ProjectName = "ST3D_Project";

typedef struct Device
{
  VkPhysicalDevice physicalDevice;
  VkPhysicalDeviceProperties properties;
  VkPhysicalDeviceFeatures features;
  VkPhysicalDeviceMemoryProperties memoryProperties;
} Device;
typedef struct App
{
  GLFWwindow *window;
  VkInstance instance;
  VkSurfaceKHR surface;
  Device device;
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