#include "queues.h"

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
  if (presentCount != 0)
  {
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentCount, details.presentModes);
  }

  return details;
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
  QueueFamilyIndices indices;
  u32 queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

  VkQueueFamilyProperties *queueFamilies = malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

  for (u32 i = 0; i < queueFamilyCount; i++)
  {
    if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
      indices.graphicsFamily = i;
    }

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

    if (presentSupport)
    {
      indices.presentFamily = i;
    }
  }

  free(queueFamilies);
  return indices;
}