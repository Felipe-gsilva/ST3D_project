#include "sync.h"

void createSyncObjects(App *pApp)
{
  VkSemaphoreCreateInfo semaphoreInfo = {
      .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};

  VkFenceCreateInfo fenceInfo = {
      .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, 
      .flags = VK_FENCE_CREATE_SIGNALED_BIT};

  if (vkCreateSemaphore(pApp->device->logicalDevice, &semaphoreInfo, NULL, &pApp->imageAvailableSemaphore) != VK_SUCCESS ||
      vkCreateSemaphore(pApp->device->logicalDevice, &semaphoreInfo, NULL, &pApp->renderFinishedSemaphore) != VK_SUCCESS ||
      vkCreateFence(pApp->device->logicalDevice, &fenceInfo, NULL, &pApp->inFlightFence) != VK_SUCCESS)
  {
    puts("failed to create synchronization objects for a frame!");
    exit(EXIT_FAILURE);
  }
}

void cleanupSyncObjects(App *pApp)
{
  vkDestroySemaphore(pApp->device->logicalDevice, pApp->renderFinishedSemaphore, NULL);
  vkDestroySemaphore(pApp->device->logicalDevice, pApp->imageAvailableSemaphore, NULL);
  vkDestroyFence(pApp->device->logicalDevice, pApp->inFlightFence, NULL);
}