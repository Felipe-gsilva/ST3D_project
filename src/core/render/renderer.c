#include "renderer.h"

const char *validationLayers[] = {
    "VK_LAYER_KHRONOS_validation"};
const u32 validationLayerCount = 1;

char *ProjectName = "ST3D_project";

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

void createInstance(App *pApp)
{
  if (enableValidationLayers && !checkValidationLayerSupport())
  {
    printf("Validation layers requested but not available!\n");
    exit(1);
  }

  VkApplicationInfo appInfo = {
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pApplicationName = ProjectName,
      .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
      .pEngineName = "No Engine",
      .engineVersion = VK_MAKE_VERSION(1, 0, 0),
      .apiVersion = VK_API_VERSION_1_0,
      .pNext = NULL};

  u32 glfwExtensionCount = 0;
  const char **glfwExtensions;

  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  const char *glfwExtensionsWithDebug[glfwExtensionCount + 1];

  for (u32 i = 0; i < glfwExtensionCount; i++)
  {
    glfwExtensionsWithDebug[i] = glfwExtensions[i];
  }
  if (enableValidationLayers)
  {
    glfwExtensionsWithDebug[glfwExtensionCount] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
  }

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {0};
  VkInstanceCreateInfo createInfo = {
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pApplicationInfo = &appInfo};

  if (enableValidationLayers)
  {
    createInfo.enabledLayerCount = validationLayerCount;
    createInfo.ppEnabledLayerNames = validationLayers;
    createInfo.enabledExtensionCount = glfwExtensionCount + 1;
    createInfo.ppEnabledExtensionNames = glfwExtensionsWithDebug;

    populateDebugMessengerCreateInfo(&debugCreateInfo);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
  }
  else
  {
    createInfo.enabledLayerCount = 0;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.pNext = NULL;
  }

  if (vkCreateInstance(&createInfo, NULL, &pApp->instance) != VK_SUCCESS)
  {
    printf("Failed to create Vulkan Instance\n");
    exit(1);
  }
}

void drawFrame(App *pApp)
{
  vkAcquireNextImageKHR(pApp->device->logicalDevice, pApp->swapChain, UINT64_MAX, pApp->imageAvailableSemaphore, VK_NULL_HANDLE, &pApp->imageIndex);
  vkWaitForFences(pApp->device->logicalDevice, 1, &pApp->inFlightFence, VK_TRUE, UINT64_MAX);
  vkResetFences(pApp->device->logicalDevice, 1, &pApp->inFlightFence);

  vkResetCommandBuffer(pApp->commandBuffer, 0);
  recordCommandBuffer(pApp);

  VkSubmitInfo submitInfo = {
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO};

  VkSemaphore waitSemaphores[] = {pApp->imageAvailableSemaphore};
  VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &pApp->commandBuffer;

  VkSemaphore signalSemaphores[] = {pApp->renderFinishedSemaphore};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  if (vkQueueSubmit(pApp->device->graphicsQueue, 1, &submitInfo, pApp->inFlightFence) != VK_SUCCESS)
  {
    puts("failed to submit draw command buffer!");
    exit(EXIT_FAILURE);
  }
}

void mainLoop(App *pApp)
{
  while (!glfwWindowShouldClose(pApp->window))
  {
    glfwPollEvents();
    drawFrame(pApp);
  }
}

void cleanup(App *pApp)
{
  cleanupSyncObjects(pApp);
  cleanupCommandbuffer(pApp);
  vkDestroyCommandPool(pApp->device->logicalDevice, pApp->commandPool, NULL);
  cleanupFramebuffers(pApp);
  vkDestroyPipeline(pApp->device->logicalDevice, pApp->graphicsPipeline, NULL);
  vkDestroyPipelineLayout(pApp->device->logicalDevice, pApp->pipelineLayout, NULL);
  vkDestroyRenderPass(pApp->device->logicalDevice, pApp->renderPass, NULL);
  if (enableValidationLayers)
  {
    DestroyDebugUtilsMessengerEXT(pApp->instance, pApp->debugMessenger, NULL);
  }
  cleanupImageViews(pApp);
  cleanupSwapChain(pApp);
  cleanupDevice(pApp);
  vkDestroySurfaceKHR(pApp->instance, pApp->surface, NULL);
  vkDestroyInstance(pApp->instance, NULL);
  destroyWindow(pApp);
}
