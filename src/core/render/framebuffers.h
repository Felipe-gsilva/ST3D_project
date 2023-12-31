#pragma once 

#include "defines.h"

void createFramebuffers(App *pApp);

void cleanupFramebuffers(App *pApp);

void createCommandBuffer(App *app);

void cleanupCommandbuffer(App *app);

void recordCommandBuffer(App *app);