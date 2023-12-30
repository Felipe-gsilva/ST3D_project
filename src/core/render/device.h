#pragma once

#include <string.h>
#include "defines.h"
#include "queues.h"



bool checkDeviceExtensionSupport(VkPhysicalDevice device);

bool isDeviceSuitable(App *pApp, VkPhysicalDevice device);

void pickPhysicalDevice(App *pApp);

void createLogicalDevice(App *pApp);

void cleanupDevice(App *pApp);