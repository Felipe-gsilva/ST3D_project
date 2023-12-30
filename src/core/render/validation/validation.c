#include "validation.h"


VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData)
{

  printf("validation layer: %s\n", pCallbackData->pMessage);

  return VK_FALSE;
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator)
{
  PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != NULL)
  {
    func(instance, debugMessenger, pAllocator);
  }
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
{
  PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

  if (func != NULL)
  {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  }
  else
  {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT *createInfo)
{
  createInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo->pfnUserCallback = debugCallback;
}

void setupDebugMessenger(App *pApp)
{
  if (!enableValidationLayers)
    return;

  VkDebugUtilsMessengerCreateInfoEXT createInfo = {0};
  populateDebugMessengerCreateInfo(&createInfo);
  // createInfo.pUserData = NULL;

  if (CreateDebugUtilsMessengerEXT(pApp->instance, &createInfo, NULL, &pApp->debugMessenger) != VK_SUCCESS)
  {
    printf("Failed to setup debug messenger!\n");
    exit(2);
  }
}

const char **getRequiredExtensions()
{
  u32 glfwExtensionCount = 0;
  const char **glfwExtensions;

  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  const char **extensions = malloc(sizeof(char *) * glfwExtensionCount);

  for (u32 i = 0; i < glfwExtensionCount; i++)
  {
    extensions[i] = glfwExtensions[i];
  }
  for(u32 i = 0; i < glfwExtensionCount; i++)
  {
    printf("Extension: %s\n", extensions[i]);
  }
  return extensions;
}

bool checkValidationLayerSupport()
{
  u32 layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, NULL);

  VkLayerProperties availableLayers[layerCount];
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

  for (u32 i = 0; i < validationLayerCount; i++)
  {
    bool layerFound = false;
    for (u32 j = 0; j < layerCount; j++)
    {
      if (strcmp(availableLayers[j].layerName, validationLayers[i]) == 0)
      {
        layerFound = true;
        break;
      }
    }
    if (!layerFound)
    {
      return false;
    }
  }
  return true;
}
