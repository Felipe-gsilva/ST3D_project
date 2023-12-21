#pragma once

#define GLFW_INCLUDE_VULKAN
#define REALEASE 0

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char *ProjectName = "ST3D_Project";

typedef struct App
{
  GLFWwindow *window;
  VkInstance instance;
} App;

typedef struct Device
{
  VkPhysicalDevice physicalDevice;
  VkDevice logicalDevice;
} Device;
