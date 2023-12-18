#ifndef DEFINES_H
#define DEFINES_H

#define GLFW_INCLUDE_VULKAN
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

#endif // DEFINES_H