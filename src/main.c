#include "main.h"

void initVulkan(App *app)
{
  {
  createInstance(app);
  // todo
  setupDebugMessenger();
  // createSurface(app);
  pickPhysicalDevice(app);
  createLogicalDevice(app);
}
}

int main()
{
  App app = {0};
  initWindow(&app);
  initVulkan(&app);
  mainLoop(&app);
  cleanup(&app);
  return EXIT_SUCCESS;
}