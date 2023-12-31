#include "command_pool.h"

void createCommandPool(App *pApp)
{
  puts("Creating Command Pool");
  QueueFamilyIndices queueFamilyIndices = findQueueFamilies(pApp->device->physicalDevice, pApp->surface);

  VkCommandPoolCreateInfo poolInfo = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
      .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
      .queueFamilyIndex = queueFamilyIndices.graphicsFamily};

  if (vkCreateCommandPool(pApp->device->logicalDevice, &poolInfo, NULL, &pApp->commandPool) != VK_SUCCESS)
  {
    puts("failed to create command pool!");
    exit(EXIT_FAILURE);
  }
}