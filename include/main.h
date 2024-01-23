#pragma once

#include "defines.h"
#include "window.h"
#include "device.h"
#include "renderer.h"
#include "swap_chain.h"
#include "pipeline.h"
#include "vertex.h"

void initVulkan(App *pApp);

int main();

void cleanup(App *pApp);
