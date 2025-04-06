#pragma once

#include "defines.h"
#include "device.h"
#include "pipeline.h"
#include "renderer.h"
#include "swap_chain.h"
#include "vertex.h"
#include "window.h"

void initVulkan(App *pApp);

int main(void);

void cleanup(App *pApp);
