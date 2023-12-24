#include "defines.h"
#include "headers/render/geometry/window.h"
#include "headers/render/geometry/renderer.h"

int main()
{
  App app = {0};
  initWindow(&app);
  initVulkan(&app);
  mainLoop(&app);
  cleanup(&app);
  return EXIT_SUCCESS;
}