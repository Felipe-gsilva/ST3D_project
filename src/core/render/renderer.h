#pragma once

#include "defines.h"
#include "queues.h"
#include "window.h"
#include "device.h"
#include "surface.h"
#include "swap_chain.h"
#include "pipeline.h"
#include "validation/validation.h"
#include "logger.h"
#include "framebuffers.h"
#include "command_pool.h"
#include "sync.h"


void createInstance(App *pApp);

void initVulkan(App *pApp);

void drawFrame(App *pApp);

void mainLoop(App *pApp);

void cleanup(App *pApp);
