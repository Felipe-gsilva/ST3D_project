#include "swap_chain.h"

u32 clamp(u32 val, u32 min, u32 max)
{
  return max(min, min(max, val));
}

VkSurfaceFormatKHR chooseSwapSurfaceFormat(u32 formatCount, VkSurfaceFormatKHR *availableFormats)
{
  for (u32 i = 0; i < formatCount; i++)
  {
    if (availableFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB && availableFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
    {
      return availableFormats[i];
    }
  }
  return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(u32 presentModeCount, VkPresentModeKHR *availablePresentModes)
{
  for (u32 i = 0; i < presentModeCount; i++)
  {
    if (availablePresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
    {
      return availablePresentModes[i];
    }
  }
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(GLFWwindow *window, VkSurfaceCapabilitiesKHR capabilities)
{
  if (capabilities.currentExtent.width != UINT_MAX)
  {
    return capabilities.currentExtent;
  }
  else
  {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D actualExtent = {(u32)width, (u32)height};

    actualExtent.width = clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
  }
}

void createSwapChain(App *pApp)
{
  SwapChainSupportDetails swapChainSupport = querySwapChainSupport(pApp->device->physicalDevice, pApp->surface);

  VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formatCount, swapChainSupport.formats);
  VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModeCount, swapChainSupport.presentModes);
  VkExtent2D extent = chooseSwapExtent(pApp->window, swapChainSupport.capabilities);

  u32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
  if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
  {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo = {
      .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
      .surface = pApp->surface,
      .minImageCount = imageCount,
      .imageFormat = surfaceFormat.format,
      .imageColorSpace = surfaceFormat.colorSpace,
      .imageExtent = extent,
      .imageArrayLayers = 1,
      .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT};

  QueueFamilyIndices indices = findQueueFamilies(pApp->device->physicalDevice, pApp->surface);
  u32 queueFamilyIndices[] = {indices.graphicsFamily, indices.presentFamily};

  if (indices.graphicsFamily != indices.presentFamily)
  {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  }
  else
  {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;  // Optional
    createInfo.pQueueFamilyIndices = NULL; // Optional
  }

  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(pApp->device->logicalDevice, &createInfo, NULL, &pApp->swapChain) != VK_SUCCESS)
  {
    printf("Failed to create Swap Chain!\n");
    exit(5);
  }

  vkGetSwapchainImagesKHR(pApp->device->logicalDevice, pApp->swapChain, &imageCount, NULL);
  pApp->swapChainImages = (VkImage *)malloc(sizeof(VkImage) * imageCount);

  vkGetSwapchainImagesKHR(pApp->device->logicalDevice, pApp->swapChain, &imageCount, pApp->swapChainImages);
  pApp->swapChainImageCount = imageCount;

  pApp->swapChainImageFormat = surfaceFormat.format;
  pApp->swapChainExtent = extent;
}

void cleanupSwapChain(App *pApp)
{
  vkDestroySwapchainKHR(pApp->device->logicalDevice, pApp->swapChain, NULL);
  free(pApp->details.formats);
  free(pApp->details.presentModes);
  free(pApp->details.extent);
  free(pApp->swapChainImages);
}