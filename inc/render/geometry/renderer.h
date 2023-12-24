#include "defines.h"
#include "render/geometry/queues.h"
#include "logger/logger.h"


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
    exit(EXIT_FAILURE);
  }
}

//----------------create surface----------------//
/*
void createSurface(App *app)
{
  if (glfwCreateWindowSurface(app->instance, app->window, NULL, &app->surface) != VK_SUCCESS)
  {
    printf("Failed to create window surface!\n");
    exit(EXIT_FAILURE);
  }

  VkBool32 presentSupport = false;
  vkGetPhysicalDeviceSurfaceSupportKHR(app->device.physicalDevice, i, app->surface, &presentSupport);
  if (presentSuport)
  {
    // indices.presentFamily = i;
  }
}
*/

bool isDeviceSuitable(App *app)
{
  QueueFamilyIndices indices = findQueueFamilies(app);

  return indices.graphicsFamily >= 0 && indices.presentFamily >= 0;
}

void pickPhysicalDevice(App *app)
{
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  uint32_t deviceCount = 0;

  vkEnumeratePhysicalDevices(app->instance, &deviceCount, NULL);
  if (!deviceCount)
  {
    puts("failed to find GPUs with Vulkan support!\n");
    exit(EXIT_FAILURE);
  }

  // FIX THIS FUNCTION
  VkPhysicalDevice *devices = malloc(sizeof(VkPhysicalDevice) * deviceCount);
  vkEnumeratePhysicalDevices(app->instance, &deviceCount, &app->device.physicalDevice);

  for (uint32_t i = 0; i < deviceCount; i++)
  {
    if (isDeviceSuitable(app))
    {
      physicalDevice = app->device.physicalDevice;
      break;
    }
  }

  if (physicalDevice == VK_NULL_HANDLE)
  {
    puts("failed to find a suitable GPU!\n");
    exit(EXIT_FAILURE);
  }
  app->device.physicalDevice = physicalDevice;
  free(devices);
}

void createLogicalDevice(App *app)
{
  float queuePriority = 1.0f;
  
  queueCreateInfo.pQueuePriorities = &queuePriority;
  VkPhysicalDeviceFeatures deviceFeatures = {0};
  VkDeviceCreateInfo createInfo = {
      .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
      .pQueueCreateInfos = &queueCreateInfo,
      .queueCreateInfoCount = 1,
      .pEnabledFeatures = &deviceFeatures};

  if (vkCreateDevice(app->device.physicalDevice, &createInfo, NULL, &app->device.physicalDevice) != VK_SUCCESS)
  {
    puts("failed to create logical device!\n");
    exit(EXIT_FAILURE);
  }
}

void initVulkan(App *app)
{
  createInstance(app);
  // todo
  setupDebugMessenger();
  //createSurface(app);
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
