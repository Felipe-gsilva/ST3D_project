#pragma once

#include "defines.h"
#include "math.h"
#include "string.h"
#include "vertex.h"

void createImageViews(App *pApp);

void ReadFile(const char *filename, ShaderFile *shader);

void createGraphicsPipeline(App *pApp);

void createRenderPass(App *pApp);

void cleanupImageViews(App *pApp);
