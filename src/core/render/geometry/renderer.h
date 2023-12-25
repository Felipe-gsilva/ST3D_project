#pragma once

#include "defines.h"
#include "geometry/queues.h"
#include "geometry/window.h"
#include "validation/validation.h"
#include "logger.h"


void createInstance(App *app);

bool isDeviceSuitable();

void pickPhysicalDevice();

void createLogicalDevice();

// todo remove this function
void setupDebugMessenger();

void initVulkan(App *app);

void mainLoop(App *app);

void cleanup(App *app);
