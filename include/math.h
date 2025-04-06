#pragma once

#include <vulkan/vulkan.h>

typedef struct vec2 {
  float x;
  float y;
} vec2;

typedef struct vec3 {
  float x;
  float y;
  float z;
} vec3;

typedef struct vec4 {
  float x;
  float y;
  float z;
  float w;
} vec4;

typedef struct Vertex {
  vec2 position;
  vec3 color;
  VkVertexInputBindingDescription bindingDescription;
  VkVertexInputAttributeDescription *attributeDescriptions;
} Vertex;
