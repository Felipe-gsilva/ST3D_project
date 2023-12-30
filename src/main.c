#include "main.h"

void initVulkan(App *pApp)
{
  {
    createInstance(pApp);
    setupDebugMessenger(pApp);
    createSurface(pApp);
    pickPhysicalDevice(pApp);
    createLogicalDevice(pApp);
    createSwapChain(pApp);
    createImageViews(pApp);
    createGraphicsPipeline(pApp);
  }
}

int main()
{
  App pApp = {0};
  initWindow(&pApp);
  initVulkan(&pApp);
  mainLoop(&pApp);
  cleanup(&pApp);
  return EXIT_SUCCESS;
}