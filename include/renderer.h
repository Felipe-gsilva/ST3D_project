#pragma once

#include "buffers.h"
#include "defines.h"
#include "device.h"
#include "logger.h"
#include "pipeline.h"
#include "queues.h"
#include "swap_chain.h"
#include "validation.h"
#include "window.h"

void createInstance(App *pApp);

void initVulkan(App *pApp);

void drawFrame(App *pApp);

void mainLoop(App *pApp);
