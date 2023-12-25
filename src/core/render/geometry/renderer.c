#include "renderer.h"

char *ProjectName = "ST3D_project";

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

bool isDeviceSuitable(VkPhysicalDevice device)
{
  VkPhysicalDeviceProperties deviceProperties;
  vkGetPhysicalDeviceProperties(device, &deviceProperties);

  VkPhysicalDeviceFeatures deviceFeatures;
  vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

  return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
         deviceFeatures.geometryShader;
}

void pickPhysicalDevice(App *app)
{
  app->device = malloc(sizeof(Device));
  app->device->physicalDevice = VK_NULL_HANDLE;
  u32 deviceCount = 0;

  vkEnumeratePhysicalDevices(app->instance, &deviceCount, NULL);
  if (deviceCount == 0)
  {
    printf("Failed to find GPUs with Vulkan support!\n");
    exit(EXIT_FAILURE);
  }

  VkPhysicalDevice *devices = malloc(sizeof(VkPhysicalDevice) * deviceCount);
  vkEnumeratePhysicalDevices(app->instance, &deviceCount, devices);

  for (u32 i = 0; i < deviceCount; i++)
  {
    if (isDeviceSuitable(devices[i]))
    {
      app->device->physicalDevice = devices[i];
      break;
    }
  }

  if (app->device->physicalDevice == VK_NULL_HANDLE)
  {
    printf("Failed to find a suitable GPU!\n");
    exit(EXIT_FAILURE);
  }
  free(devices);
}

void createLogicalDevice()
{
  // todo create logical device
  printf("create logical device\n");
}

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

  if (result != VK_SUCCESS)
  {
    printf("Failed to create instance!\n");
    exit(EXIT_FAILURE);
  }

  u32 extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
  VkExtensionProperties *extensions = malloc(sizeof(VkExtensionProperties) * extensionCount);
  vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensions);

  printf("available extensions:\n");
  for (u32 i = 0; i < extensionCount; i++)
  {
    
    printf("\t%s\n", extensions[i].extensionName);
  }

  if (enableValidationLayers && !checkValidationLayerSupport())
  {
    printf("validation layers requested, but not available!\n");
    exit(EXIT_FAILURE);
  }

  // add validation layers to instance creation info if enabled
  free(extensions);
}

void setupDebugMessenger()
{
  if (!glfwVulkanSupported())
  {
    printf("Vulkan is not supported!\n");
    exit(EXIT_FAILURE);
  }
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
  destroyWindow(app);
  free(app->device);
  vkDestroyInstance(app->instance, NULL);
  vkDestroySurfaceKHR(app->instance, app->surface, NULL); 
}
