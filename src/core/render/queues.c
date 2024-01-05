#include "queues.h"

// ------------------------------------------------- //
// -------------  Swap Chain Suport ---------------- //

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{
  SwapChainSupportDetails details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

  u32 formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, NULL);
  details.formatCount = formatCount;
  VkSurfaceFormatKHR formats[formatCount];
  details.formats = formats;
  if (formatCount != 0)
  {
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats);
  }

  u32 presentCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentCount, NULL);
  details.presentModeCount = presentCount;
  VkPresentModeKHR presentModes[presentCount];
  details.presentModes = presentModes;
  printf("presentCount: %d\n", presentCount);
  if (presentCount != 0)
  {
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentCount, details.presentModes);
  }

  return details;
}

// ------------------------------------------------- //
// -------------  Find Queue Families -------------- //

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
  QueueFamilyIndices indices = {0};

  u32 queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

  VkQueueFamilyProperties queueFamilyProperties[queueFamilyCount];
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyProperties);

  for (u32 i = 0; i < queueFamilyCount; i++)
  {
    if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
      indices.graphicsFamily = i;
      indices.isGraphicsFamilySet = true;
    }
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
    if (presentSupport)
    {
      indices.presentFamily = i;
      indices.isPresentFamilySet = true;
    }
  }
  return indices;
}