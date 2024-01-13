#include "window.h"

void createSurface(App *pApp)
{
  puts("Creating Surface");
  if (glfwCreateWindowSurface(pApp->instance, pApp->window, NULL, &pApp->surface) != VK_SUCCESS)
  {
    printf("Failed to create window surface!\n");
    exit(EXIT_FAILURE);
  }
  puts("Surface created!");
}

void initWindow(App *pApp)
{
  const u32 WIDTH = 800;
  const u32 HEIGHT = 600;

  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  pApp->window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", NULL, NULL);
}

void destroyWindow(App *pApp)
{
  glfwDestroyWindow(pApp->window);
  glfwTerminate();
  puts("Window destroyed!");
}

