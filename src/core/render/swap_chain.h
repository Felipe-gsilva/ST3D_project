#pragma once


#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#include "defines.h"
#include "queues.h"
#include <stdlib.h>

u32 clamp(u32 val, u32 min, u32 max);

VkSurfaceFormatKHR chooseSwapSurfaceFormat(u32 formatCount, VkSurfaceFormatKHR *availableFormats);

VkPresentModeKHR chooseSwapPresentMode(u32 presentModeCount, VkPresentModeKHR *availablePresentModes);

VkExtent2D chooseSwapExtent(GLFWwindow *window, VkSurfaceCapabilitiesKHR capabilities);

void createSwapChain(App *pApp);

void cleanupSwapChain(App *pApp);