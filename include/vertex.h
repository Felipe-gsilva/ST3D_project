#pragma once

#include "defines.h"
#include "math.h"
#include "string.h"

void getBindingDescription(VkVertexInputBindingDescription *bindingDescription);

void getAttributeDescriptions(VkVertexInputAttributeDescription *attributeDescriptions);

void createVertexBuffer(App *pApp);

void fillVertices(Vertex *vertex);

u32 findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, App *pApp);

void cleanupVertexBuffer(App *pApp);

