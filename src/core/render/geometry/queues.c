#include "queues.h"

QueueFamilyIndices findQueueFamilies(App *app)
{
  printf("find queue families\n");
  QueueFamilyIndices indices;
  u32 queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(app->device->physicalDevice, &queueFamilyCount, NULL);

  VkQueueFamilyProperties *queueFamilies = malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));
  vkGetPhysicalDeviceQueueFamilyProperties(app->device->physicalDevice, &queueFamilyCount, queueFamilies);

  for (u32 i = 0; i < queueFamilyCount; i++)
  {
    if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
      indices.graphicsFamily = i;
    }
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(app->device->physicalDevice, i, app->surface, &presentSupport);
    if (presentSupport)
    {
      indices.presentFamily = i;
    }
  }
  
  free(queueFamilies);
  return indices;
}