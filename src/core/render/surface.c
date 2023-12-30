#include "surface.h"

void createSurface(App *pApp)
{;
  if (glfwCreateWindowSurface(pApp->instance, pApp->window, NULL, &pApp->surface) != VK_SUCCESS)
  {
    printf("Failed to create window surface!\n");
    exit(EXIT_FAILURE);
  }
  puts("Surface created!");
}