#include "window.h"

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
  App* pApp = (App*)glfwGetWindowUserPointer(window);
  pApp->framebufferResized = true;
}

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

  pApp->window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", NULL, NULL);
  glfwSetWindowUserPointer(pApp->window, pApp);
  glfwSetFramebufferSizeCallback(pApp->window, framebufferResizeCallback);
}

void destroyWindow(App *pApp)
{
  glfwDestroyWindow(pApp->window);
  glfwTerminate();
  puts("Window destroyed!");
}

