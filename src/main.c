#include "defines.h"
#include "render/geometry/window.h"
#include "render/geometry/renderer.h"

int main()
{
  App app = {0};
  initWindow(&app);
  initVulkan(&app);
  mainLoop(&app);
  cleanup(&app);
  return EXIT_SUCCESS;
}