#include "defines.h"

QueueFamilyIndices findQueueFamilies(App *app)
{
  QueueFamilyIndices indices;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(app->device.physicalDevice, &queueFamilyCount, NULL);

  VkQueueFamilyProperties *queueFamilies = (VkQueueFamilyProperties *)malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));
  vkGetPhysicalDeviceQueueFamilyProperties(app->device.physicalDevice, &queueFamilyCount, queueFamilies);

  for (uint32_t i = 0; i < queueFamilyCount; i++)
  {
    if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
      indices.graphicsFamily = i;
    }

    if (indices.graphicsFamily >= 0)
    {
      break;
    }
  }
  return indices;
}