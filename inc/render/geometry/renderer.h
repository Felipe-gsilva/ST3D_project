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

void createSurface(App *app)
{
  if (glfwCreateWindowSurface(app->instance, app->window, NULL, &app->surface) != VK_SUCCESS)
  {
    printf("Failed to create window surface!\n");
    exit(EXIT_FAILURE);
  }

  VkBool32 presentSupport = false;
  vkGetPhysicalDeviceSurfaceSupportKHR(app->device.physicalDevice, i, app->surface, &presentSupport);
  if (!presentSupport)
  {
    printf("Physical device does not support presentation!\n");
    exit(EXIT_FAILURE);
  }
}

int isDeviceSuitable(VkPhysicalDevice device)
{
  VkPhysicalDeviceProperties deviceProperties;
  vkGetPhysicalDeviceProperties(device, &deviceProperties);
  VkPhysicalDeviceFeatures deviceFeatures;
  vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

  return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
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

  // save on instance//
  VkPhysicalDevice *devices = malloc(sizeof(VkPhysicalDevice) * deviceCount);
  vkEnumeratePhysicalDevices(app->instance, &deviceCount, devices);

  for (uint32_t i = 0; i < deviceCount; i++)
  {
    if (isDeviceSuitable(devices[i]))
    {
      physicalDevice = devices[i];
      break;
    }
  }

  if (physicalDevice == VK_NULL_HANDLE)
  {
    puts("failed to find a suitable GPU!\n");
    exit(EXIT_FAILURE);
  }
  app->device.physicalDevice = physicalDevice;
}

void createLogicalDevice()
{
  // todo
}

void initVulkan(App *app)
{
  createInstance(app);
  // todo
  setupDebugMessenger();
  createSurface(app);
  pickPhysicalDevice(app);
  createLogicalDevice();
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
