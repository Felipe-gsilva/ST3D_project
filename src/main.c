#include "main.h"

// ------------------------------------------------- //
// ------------------Init Vulkan ------------------- //

void initVulkan(App *pApp) {
  {
    createInstance(pApp);
    setupDebugMessenger(pApp);
    createSurface(pApp);
    pickPhysicalDevice(pApp);
    createLogicalDevice(pApp);
    createSwapChain(pApp);
    createImageViews(pApp);
    createGraphicsPipeline(pApp);
    createFramebuffers(pApp);
    createCommandPool(pApp);
    createVertexBuffer(pApp);
    createCommandBuffer(pApp);
    createSyncObjects(pApp);
  }
}

// ------------------------------------------------- //
// ------------------  Clean Up  ------------------- //

void cleanup(App *pApp) {
  cleanupSwapChain(pApp);
  cleanupVertexBuffer(pApp);
  cleanupSyncObjects(pApp);
  cleanupCommandbuffer(pApp);
  vkDestroyCommandPool(pApp->logicalDevice, pApp->commandPool, NULL);
  vkDestroyPipeline(pApp->logicalDevice, pApp->graphicsPipeline, NULL);
  vkDestroyPipelineLayout(pApp->logicalDevice, pApp->pipelineLayout, NULL);
  vkDestroyRenderPass(pApp->logicalDevice, pApp->renderPass, NULL);
  if (enableValidationLayers) {
    DestroyDebugUtilsMessengerEXT(pApp->instance, pApp->debugMessenger, NULL);
  }
  cleanupDevice(pApp);
  vkDestroySurfaceKHR(pApp->instance, pApp->surface, NULL);
  vkDestroyInstance(pApp->instance, NULL);
  destroyWindow(pApp);
}

int main(void) {
  App pApp = {0};
  initWindow(&pApp);
  initVulkan(&pApp);
  mainLoop(&pApp);
  cleanup(&pApp);
  return EXIT_SUCCESS;
}
