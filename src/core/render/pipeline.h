#pragma once

#include "defines.h"
#include "string.h"

void createImageViews(App *pApp);

void ReadFile(const char *filename, ShaderFile *shader);

void createGraphicsPipeline(App *pApp);

void createGraphicsQueue();

void cleanupImageViews(App *pApp);