#pragma once

#include "defines.h"
#include "queues.h"

void createCommandPool(App *pApp);

void createFramebuffers(App *pApp);

void cleanupFramebuffers(App *pApp);

void createCommandBuffer(App *pApp);

void cleanupCommandbuffer(App *pApp);

void recordCommandBuffer(App *pApp, VkCommandBuffer commandBuffer,
                         u32 imageIndex);

void createSyncObjects(App *pApp);

void cleanupSyncObjects(App *pApp);
