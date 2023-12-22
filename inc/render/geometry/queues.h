#include "defines.h"

typedef struct QueueFamilyIndices
{
  uint32_t graphicsFamily;
} QueueFamilyIndices;

QueueFamilyIndices findQueueFamilies(App *app)
{
  QueueFamilyIndices indices;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(app->device.physicalDevice, &queueFamilyCount, NULL);

  VkQueueFamilyProperties *queueFamilies = (VkQueueFamilyProperties *)malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));
  vkGetPhysicalDeviceQueueFamilyProperties(app->device.physicalDevice, &queueFamilyCount, queueFamilies);

  return indices;
}