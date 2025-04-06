#include "renderer.h"

const int MAX_FRAMES_IN_FLIGHT = 2;
const char *validationLayers[] = {"VK_LAYER_KHRONOS_validation"};
const u32 validationLayerCount = 1;
u32 currentFrame = 0;
const char ProjectName[] = "ST3D_project";
bool framebufferResized = false;

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

// ------------------------------------------------- //
// ------------------  Instance  ------------------- //

void createInstance(App *pApp) {
  if (enableValidationLayers && !checkValidationLayerSupport()) {
    printf("Validation layers requested but not available!\n");
    exit(1);
  }

  VkApplicationInfo appInfo = {.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
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

  for (u32 i = 0; i < glfwExtensionCount; i++) {
    glfwExtensionsWithDebug[i] = glfwExtensions[i];
  }
  if (enableValidationLayers) {
    glfwExtensionsWithDebug[glfwExtensionCount] =
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
  }

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {0};
  VkInstanceCreateInfo createInfo = {.sType =
                                         VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                     .pApplicationInfo = &appInfo};

  if (enableValidationLayers) {
    createInfo.enabledLayerCount = validationLayerCount;
    createInfo.ppEnabledLayerNames = validationLayers;
    createInfo.enabledExtensionCount = glfwExtensionCount + 1;
    createInfo.ppEnabledExtensionNames = glfwExtensionsWithDebug;

    populateDebugMessengerCreateInfo(&debugCreateInfo);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.pNext = NULL;
  }

  if (vkCreateInstance(&createInfo, NULL, &pApp->instance) != VK_SUCCESS) {
    printf("Failed to create Vulkan Instance\n");
    exit(1);
  }
}

// ------------------------------------------------- //
// ------------------ Draw Frames ------------------ //

void drawFrame(App *pApp) {
  vkWaitForFences(pApp->logicalDevice, 1, &pApp->inFlightFence[currentFrame],
                  VK_TRUE, UINT64_MAX);

  u32 imageIndex;
  VkResult result = vkAcquireNextImageKHR(
      pApp->logicalDevice, pApp->swapChain, UINT64_MAX,
      pApp->imageAvailableSemaphore[currentFrame], VK_NULL_HANDLE, &imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      framebufferResized) {
    framebufferResized = false;
    recreateSwapChain(pApp);
  } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    printf("Failed to acquire swap chain image!\n");
  }

  vkResetFences(pApp->logicalDevice, 1, &pApp->inFlightFence[currentFrame]);

  vkResetCommandBuffer(pApp->commandBuffer[currentFrame], 0);
  recordCommandBuffer(pApp, pApp->commandBuffer[currentFrame], imageIndex);

  VkSubmitInfo submitInfo = {.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO};

  VkSemaphore waitSemaphores[] = {pApp->imageAvailableSemaphore[currentFrame]};
  VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &pApp->commandBuffer[currentFrame];

  VkSemaphore signalSemaphores[] = {
      pApp->renderFinishedSemaphore[currentFrame]};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  if (vkQueueSubmit(pApp->graphicsQueue, 1, &submitInfo,
                    pApp->inFlightFence[currentFrame]) != VK_SUCCESS) {
    printf("Failed to submit draw command buffer!\n");
    exit(EXIT_FAILURE);
  }

  currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

  VkPresentInfoKHR presentInfo = {.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
                                  .waitSemaphoreCount = 1,
                                  .pWaitSemaphores = signalSemaphores};
  VkSwapchainKHR swapChains[] = {pApp->swapChain};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageIndex;
  presentInfo.pResults = NULL;

  vkQueuePresentKHR(pApp->presentQueue, &presentInfo);
}

// ------------------------------------------------- //
// ------------------ Main Loop -------------------- //

void mainLoop(App *pApp) {
  while (!glfwWindowShouldClose(pApp->window)) {
    glfwPollEvents();
    drawFrame(pApp);
  }
  vkDeviceWaitIdle(pApp->logicalDevice);
}
