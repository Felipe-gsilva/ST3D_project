#include "renderer.h"

const char *validationLayers[] = {
    "VK_LAYER_KHRONOS_validation"};
const u32 validationLayerCount = 1;

char *ProjectName = "ST3D_project";

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

void createInstance(App *pApp)
{
  if (enableValidationLayers && !checkValidationLayerSupport())
  {
    printf("Validation layers requested but not available!\n");
    exit(1);
  }

  VkApplicationInfo appInfo = {
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pApplicationName = ProjectName,
      .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
      .pEngineName = "No Engine",
      .engineVersion = VK_MAKE_VERSION(1, 0, 0),
      .apiVersion = VK_API_VERSION_1_0,
      .pNext = NULL};

  u32 glfwExtensionCount = 0;
  const char **glfwExtensions;

  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  const char *glfwExtensionsWithDebug[glfwExtensionCount + 1];

  for (u32 i = 0; i < glfwExtensionCount; i++)
  {
    glfwExtensionsWithDebug[i] = glfwExtensions[i];
  }
  if (enableValidationLayers)
  {
    glfwExtensionsWithDebug[glfwExtensionCount] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
  }

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {0};
  VkInstanceCreateInfo createInfo = {
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pApplicationInfo = &appInfo};

  if (enableValidationLayers)
  {
    createInfo.enabledLayerCount = validationLayerCount;
    createInfo.ppEnabledLayerNames = validationLayers;
    createInfo.enabledExtensionCount = glfwExtensionCount + 1;
    createInfo.ppEnabledExtensionNames = glfwExtensionsWithDebug;

    populateDebugMessengerCreateInfo(&debugCreateInfo);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
  }
  else
  {
    createInfo.enabledLayerCount = 0;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.pNext = NULL;
  }

  if (vkCreateInstance(&createInfo, NULL, &pApp->instance) != VK_SUCCESS)
  {
    printf("Failed to create Vulkan Instance\n");
    exit(1);
  }
}

void mainLoop(App *pApp)
{
  while (!glfwWindowShouldClose(pApp->window))
  {
    glfwPollEvents();
  }
}

void cleanup(App *pApp)
{
  if (enableValidationLayers)
  {
    DestroyDebugUtilsMessengerEXT(pApp->instance, pApp->debugMessenger, NULL);
  }
  cleanupImageViews(pApp);
  cleanupSwapChain(pApp);
  cleanupDevice(pApp);
  vkDestroySurfaceKHR(pApp->instance, pApp->surface, NULL);
  vkDestroyInstance(pApp->instance, NULL);
  destroyWindow(pApp);
}
