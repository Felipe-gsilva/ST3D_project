#include "defines.h"

void initWindow(App *app)
{
  const uint32_t WIDTH = 800;
  const uint32_t HEIGHT = 600;

  if (!glfwInit())
    exit(1);

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  app->window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", NULL, NULL);
}
void mainLoop(App *app)
{
  while (!glfwWindowShouldClose(app->window))
  {
    glfwPollEvents();
  }
}
