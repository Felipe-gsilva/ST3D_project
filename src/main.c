#include "defines.h"
#include "render/window.h"
#include "render/renderer.h"

int main()
{
  App app = {0};
  initWindow(&app);
  initVulkan(&app);
  mainLoop(&app);
  cleanup(&app);
  return EXIT_SUCCESS;
}