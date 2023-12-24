#include "defines.h"
#include "render/geometry/queues.h"
#include "render/geometry/surface.h"
#include "logger/logger.h"

void createInstance(App *app)
{

  VkApplicationInfo appInfo = {
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pApplicationName = ProjectName,
      .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
      .pEngineName = "No Engine",
      .engineVersion = VK_MAKE_VERSION(1, 0, 0),
      .apiVersion = VK_API_VERSION_1_0};

  const char **glfwExtensions;
  u32 glfwExtensionCount = 0;

  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

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
    exit(EXIT_FAILURE);
  }
}

bool isDeviceSuitable(App *app)
{
  QueueFamilyIndices indices = findQueueFamilies(app);

  return indices.graphicsFamily >= 0 && indices.presentFamily >= 0;
}

void pickPhysicalDevice(App *app)
{
  printf("pick physical device\n");
}

void createLogicalDevice(App *app)
{
  // todo create logical device
  printf("create logical device\n");
}

// todo - remove this function
void setupDebugMessenger()
{
  if (!glfwVulkanSupported())
  {
    printf("Vulkan is not supported!\n");
    exit(EXIT_FAILURE);
  }
}
void initVulkan(App *app)
{
  createInstance(app);
  // todo
  setupDebugMessenger();
  // createSurface(app);
  pickPhysicalDevice(app);
  createLogicalDevice(app);
}

void mainLoop(App *app)
{
  while (!glfwWindowShouldClose(app->window))
  {
    glfwPollEvents();
  }
}

void cleanup(App *app)
{
  vkDestroyInstance(app->instance, NULL);
  vkDestroySurfaceKHR(app->instance, app->surface, NULL);
  destroyWindow(app);
}
