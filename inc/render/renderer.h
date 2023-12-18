#include "defines.h"

void createInstance(App *app)
{
  const char **glfwExtensions;
  uint32_t glfwExtensionCount = 0;

  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  VkApplicationInfo appInfo = {
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pApplicationName = ProjectName,
      .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
      .pEngineName = "No Engine",
      .engineVersion = VK_MAKE_VERSION(1, 0, 0),
      .apiVersion = VK_API_VERSION_1_0};

  VkInstanceCreateInfo createInfo = {
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pApplicationInfo = &appInfo,
      .enabledExtensionCount = glfwExtensionCount,
      .ppEnabledExtensionNames = glfwExtensions,
      .enabledLayerCount = 0};

  VkResult result = vkCreateInstance(&createInfo, NULL, &app->instance);

  if (vkCreateInstance(&createInfo, NULL, &app->instance) != VK_SUCCESS)
  {
    printf("Failed to create instance!\n");
  }
}

void initVulkan(App *app)
{
  createInstance(app);
}
void cleanup(App *app)
{
  vkDestroyInstance(app->instance, NULL);

  glfwDestroyWindow(app->window);
  glfwTerminate();
}