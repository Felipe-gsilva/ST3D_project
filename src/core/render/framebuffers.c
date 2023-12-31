#include "framebuffers.h"

void createFramebuffers(App *pApp)
{
  puts("Creating Framebuffers");
  size_t swapChainImageCount = pApp->swapChainImageCount;

  pApp->swapChainFramebuffers = malloc(sizeof(VkFramebuffer *) * swapChainImageCount);

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

    if (vkCreateFramebuffer(pApp->device->logicalDevice, &framebufferInfo, NULL, &pApp->swapChainFramebuffers[i]) != VK_SUCCESS)
    {
      puts("failed to create framebuffer!");
      exit(EXIT_FAILURE);
    }
  }
}

void cleanupFramebuffers(App *pApp)
{
  for (size_t i = 0; i < pApp->swapChainImageCount; i++)
  {
    vkDestroyFramebuffer(pApp->device->logicalDevice, pApp->swapChainFramebuffers[i], NULL);
  }

  free(pApp->swapChainFramebuffers);
}

void createCommandBuffer(App *pApp)
{
  printf("Creating Command Buffer\n");
  pApp->commandBuffer = malloc(sizeof(VkCommandBuffer) * pApp->swapChainImageCount);

  VkCommandBufferAllocateInfo allocInfo = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
      .commandPool = pApp->commandPool,
      .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
      .commandBufferCount = 1};

  if (vkAllocateCommandBuffers(pApp->device->logicalDevice, &allocInfo, &pApp->commandBuffer) != VK_SUCCESS)
  {
    puts("failed to allocate command buffers!");
    exit(EXIT_FAILURE);
  }
}

void cleanupCommandbuffer(App *pApp)
{
  free(pApp->commandBuffer);
}

void recordCommandBuffer(App *pApp)
{
  VkCommandBufferBeginInfo beginInfo = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = 0,
      .pInheritanceInfo = NULL};

  if (vkBeginCommandBuffer(pApp->commandBuffer, &beginInfo) != VK_SUCCESS)
  {
    puts("failed to begin recording command buffer!");
    exit(EXIT_FAILURE);
  }

  VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};

  VkRenderPassBeginInfo renderPassInfo = {
      .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
      .renderPass = pApp->renderPass,
      .framebuffer = pApp->swapChainFramebuffers[pApp->imageIndex],
      .renderArea = {.offset = {0, 0},
                     .extent = pApp->swapChainExtent},
      .clearValueCount = 1,
      .pClearValues = &clearColor};

  vkCmdBeginRenderPass(pApp->commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

  vkCmdBindPipeline(pApp->commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pApp->graphicsPipeline);

  VkViewport viewport = {
      .x = 0.0f,
      .y = 0.0f,
      .width = (float)pApp->swapChainExtent.width,
      .height = (float)pApp->swapChainExtent.height,
      .minDepth = 0.0f,
      .maxDepth = 1.0f};
  vkCmdSetViewport(pApp->commandBuffer, 0, 1, &viewport);

  VkRect2D scissor = {
      .offset = {0, 0},
      .extent = pApp->swapChainExtent};
  vkCmdSetScissor(pApp->commandBuffer, 0, 1, &scissor);

  vkCmdDraw(pApp->commandBuffer, 3, 1, 0, 0);

  vkCmdEndRenderPass(pApp->commandBuffer);

  if (vkEndCommandBuffer(pApp->commandBuffer) != VK_SUCCESS)
  {
    puts("failed to record command buffer!");
    exit(EXIT_FAILURE);
  }
}