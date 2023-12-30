#include "device.h"

bool checkDeviceExtensionSupport(VkPhysicalDevice device)
{
  u32 extensionCount;
  vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, NULL);
  VkExtensionProperties availableExtensions[extensionCount];

  vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, availableExtensions);

  char *deviceExtensions[] = {"VK_KHR_swapchain"};

  size_t requiredExtensionsCount = sizeof(deviceExtensions) / sizeof(deviceExtensions[0]);

  for (u32 i = 0; i < extensionCount; i++)
  {
    for (size_t j = 0; j < requiredExtensionsCount; j++)
    {
      if (strcmp(availableExtensions[i].extensionName, deviceExtensions[j]) == 0)
      {
        requiredExtensionsCount--;
        printf("---Extension: %s\n", availableExtensions[i].extensionName);
        break;
      }
    }
  }
  return (requiredExtensionsCount == 0);
}

bool isDeviceSuitable(App *pApp, VkPhysicalDevice device)
{
  QueueFamilyIndices indices = findQueueFamilies(device, pApp->surface);

  vkGetPhysicalDeviceProperties(device, &pApp->device->properties);

  vkGetPhysicalDeviceFeatures(device, &pApp->device->features);

  bool extensionsSupported = checkDeviceExtensionSupport(device);

  bool swapChainAdequate = false;

  if (extensionsSupported)
  {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, pApp->surface);
    swapChainAdequate = swapChainSupport.formats && swapChainSupport.presentModes;
    puts("Swap chain support checked!");
  }

  return indices.isPresentFamilySet && extensionsSupported && swapChainAdequate;
}

void pickPhysicalDevice(App *pApp)
{
  pApp->device = malloc(sizeof(Device));
  pApp->device->physicalDevice = VK_NULL_HANDLE;
  u32 deviceCount = 0;

  vkEnumeratePhysicalDevices(pApp->instance, &deviceCount, NULL);
  if (deviceCount == 0)
  {
    printf("Failed to find GPUs with Vulkan support!\n");
    exit(EXIT_FAILURE);
  }

  pApp->deviceList = malloc(sizeof(VkPhysicalDevice) * deviceCount);
  vkEnumeratePhysicalDevices(pApp->instance, &deviceCount, pApp->deviceList);

  printf("Found %d devices!\n", deviceCount);

  for (u32 i = 0; i < deviceCount; i++)
  {
    if (isDeviceSuitable(pApp, pApp->deviceList[i]))
    {
      pApp->device->physicalDevice = pApp->deviceList[i];
      break;
    }
  }

  if (pApp->device->physicalDevice == VK_NULL_HANDLE)
  {
    printf("Failed to find a suitable GPU!\n");
    exit(EXIT_FAILURE);
  }

  free(pApp->deviceList);
  printf("Physical device picked!\n");
}

void createLogicalDevice(App *pApp)
{
  QueueFamilyIndices indices = findQueueFamilies(pApp->device->physicalDevice, pApp->surface);
  float queuePriority = 1.0f;

  VkDeviceQueueCreateInfo queueCreateInfos[2];
  u32 uniqueQueueFamilies[2] = {indices.graphicsFamily, indices.presentFamily};
  u32 queueCreateInfoCount = 0;

  for (u32 i = 0; i < 2; i++)
  {
    if (uniqueQueueFamilies[i] != UINT32_MAX)
    {
      VkDeviceQueueCreateInfo queueCreateInfo = {0};
      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = uniqueQueueFamilies[i];
      queueCreateInfo.queueCount = 1;
      queueCreateInfo.pQueuePriorities = &queuePriority;
      queueCreateInfos[queueCreateInfoCount++] = queueCreateInfo;
    }
  }

  VkPhysicalDeviceFeatures deviceFeatures;
  vkGetPhysicalDeviceFeatures(pApp->device->physicalDevice, &deviceFeatures);

  VkDeviceCreateInfo createInfo = {
      .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
      .pQueueCreateInfos = queueCreateInfos,
      .queueCreateInfoCount = queueCreateInfoCount,
      .pEnabledFeatures = &deviceFeatures,
      .enabledExtensionCount = 1,
      .ppEnabledExtensionNames = (const char *const[]){VK_KHR_SWAPCHAIN_EXTENSION_NAME}};

  if (enableValidationLayers)
  {
    createInfo.enabledLayerCount = validationLayerCount;
    createInfo.ppEnabledLayerNames = validationLayers;
  }
  else
  {
    createInfo.enabledLayerCount = 0;
  }

  if (vkCreateDevice(pApp->device->physicalDevice, &createInfo, NULL, &pApp->device->logicalDevice) != VK_SUCCESS)
  {
    puts("failed to create logical device!");
    exit(EXIT_FAILURE);
  }

  vkGetDeviceQueue(pApp->device->logicalDevice, indices.graphicsFamily, 0, &pApp->device->presentQueue);
  printf("Logical device created!\n");
}

void cleanupDevice(App *pApp)
{
  vkDestroyDevice(pApp->device->logicalDevice, NULL);
  free(pApp->device);
  printf("Logical device destroyed!\n");
}