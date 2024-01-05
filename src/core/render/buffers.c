#include "buffers.h"

void createCommandPool(App *pApp)
{
  puts("Creating Command Pool");
  QueueFamilyIndices queueFamilyIndices = findQueueFamilies(pApp->physicalDevice, pApp->surface);

  VkCommandPoolCreateInfo poolInfo = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
      .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
      .queueFamilyIndex = queueFamilyIndices.graphicsFamily};

  if (vkCreateCommandPool(pApp->logicalDevice, &poolInfo, NULL, &pApp->commandPool) != VK_SUCCESS)
  {
    puts("failed to create command pool!");
    exit(EXIT_FAILURE);
  }
  puts("Command Pool Created");
}

void createFramebuffers(App *pApp)
{
  puts("Creating Framebuffers");
  size_t swapChainImageCount = pApp->swapChainImageCount;
  printf("swap chain image count: %ld\n", swapChainImageCount);

  pApp->swapChainFramebuffers = (VkFramebuffer *)malloc(sizeof(VkFramebuffer) * swapChainImageCount);

  for (size_t i = 0; i < swapChainImageCount; i++)
  {
    VkImageView attachments[] = {pApp->swapChainImageViews[i]};

    VkFramebufferCreateInfo framebufferInfo = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass = pApp->renderPass,
        .attachmentCount = 1,
        .pAttachments = attachments,
        .width = pApp->swapChainExtent.width,
        .height = pApp->swapChainExtent.height,
        .layers = 1};

    if (vkCreateFramebuffer(pApp->logicalDevice, &framebufferInfo, NULL, &pApp->swapChainFramebuffers[i]) != VK_SUCCESS)
    {
      puts("failed to create framebuffer!");
      exit(EXIT_FAILURE);
    }
  }
  puts("Framebuffers Created");
}

void cleanupFramebuffers(App *pApp)
{
  for (size_t i = 0; i < pApp->swapChainImageCount; i++)
  {
    vkDestroyFramebuffer(pApp->logicalDevice, pApp->swapChainFramebuffers[i], NULL);
  }
  free(pApp->swapChainFramebuffers);
}

void cleanupCommandbuffer(App *pApp)
{
  vkFreeCommandBuffers(pApp->logicalDevice, pApp->commandPool, (u32)MAX_FRAMES_IN_FLIGHT, pApp->commandBuffer);
}

void recordCommandBuffer(App *pApp, VkCommandBuffer commandBuffer, u32 imageIndex)
{
  VkCommandBufferBeginInfo beginInfo = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = 0,
      .pInheritanceInfo = NULL};

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
  {
    printf("failed to begin recording command buffer!\n");
    exit(13);
  }

  VkRenderPassBeginInfo renderPassInfo = {
      .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
      .renderPass = pApp->renderPass,
      .framebuffer = pApp->swapChainFramebuffers[imageIndex],
      .renderArea.offset.x = 0,
      .renderArea.offset.y = 0,
      .renderArea.extent = pApp->swapChainExtent};

  VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
  renderPassInfo.clearValueCount = 1;
  renderPassInfo.pClearValues = &clearColor;

  vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pApp->graphicsPipeline);

  VkViewport viewport = {
      .x = 0.0f,
      .y = 0.0f,
      .width = (float)pApp->swapChainExtent.width,
      .height = (float)pApp->swapChainExtent.height,
      .minDepth = 0.0f,
      .maxDepth = 1.0f};
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

  VkRect2D scissor = {
      .offset.x = 0,
      .offset.y = 0,
      .extent = pApp->swapChainExtent};
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

  vkCmdDraw(commandBuffer, 3, 1, 0, 0);

  vkCmdEndRenderPass(commandBuffer);

  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
  {
    printf("failed to record command buffer!\n");
    exit(14);
  }
  puts("Command Buffer Recorded");
}

void createCommandBuffer(App *pApp)
{
  printf("Creating Command Buffer\n");
  pApp->commandBuffer = (VkCommandBuffer *)malloc(sizeof(VkCommandBuffer) * MAX_FRAMES_IN_FLIGHT);
  VkCommandBufferAllocateInfo allocInfo = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
      .commandPool = pApp->commandPool,
      .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
      .commandBufferCount = MAX_FRAMES_IN_FLIGHT};

  if (vkAllocateCommandBuffers(pApp->logicalDevice, &allocInfo, pApp->commandBuffer) != VK_SUCCESS)
  {
    puts("failed to allocate command buffers!");
    exit(EXIT_FAILURE);
  }
  puts("Command Buffer Created");
}

void createSyncObjects(App *pApp)
{
  pApp->imageAvailableSemaphore = (VkSemaphore *)calloc(sizeof(VkSemaphore), MAX_FRAMES_IN_FLIGHT);
  pApp->renderFinishedSemaphore = (VkSemaphore *)calloc(sizeof(VkSemaphore), MAX_FRAMES_IN_FLIGHT);
  pApp->inFlightFence = (VkFence *)calloc(sizeof(VkFence), MAX_FRAMES_IN_FLIGHT);

  VkSemaphoreCreateInfo semaphoreInfo = {
      .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};

  VkFenceCreateInfo fenceInfo = {
      .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
      .flags = VK_FENCE_CREATE_SIGNALED_BIT};

  for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
  {
    if (vkCreateSemaphore(pApp->logicalDevice, &semaphoreInfo, NULL, &pApp->imageAvailableSemaphore[i]) != VK_SUCCESS)
    {
      printf("Failed to create imageAvailableSemaphore!\n");
      exit(15);
    }
    if (vkCreateSemaphore(pApp->logicalDevice, &semaphoreInfo, NULL, &pApp->renderFinishedSemaphore[i]) != VK_SUCCESS)
    {
      printf("Failed to create renderFinishedSemaphore!\n");
      exit(15);
    }
    if (vkCreateFence(pApp->logicalDevice, &fenceInfo, NULL, &pApp->inFlightFence[i]) != VK_SUCCESS)
    {
      printf("Failed to create fence!\n");
      exit(15);
    }
  }
}

void cleanupSyncObjects(App *pApp)
{
  for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
  {
    vkDestroySemaphore(pApp->logicalDevice, pApp->renderFinishedSemaphore[i], NULL);
    vkDestroySemaphore(pApp->logicalDevice, pApp->imageAvailableSemaphore[i], NULL);
    vkDestroyFence(pApp->logicalDevice, pApp->inFlightFence[i], NULL);
  }
}