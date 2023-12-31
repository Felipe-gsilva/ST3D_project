#pragma once

#include "defines.h"
#include "string.h"

void createImageViews(App *pApp);

void ReadFile(const char *filename, ShaderFile *shader);

void createGraphicsPipeline(App *pApp);

void createRenderPass(App *pApp);

void cleanupImageViews(App *pApp);