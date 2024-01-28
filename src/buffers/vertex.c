#include "vertex.h"

void getBindingDescription(VkVertexInputBindingDescription *bindingDescription)
{
  bindingDescription->binding = 0;
  bindingDescription->stride = sizeof(Vertex);
  bindingDescription->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
}

void getAttributeDescriptions(VkVertexInputAttributeDescription *attributeDescriptions)
{
  attributeDescriptions[0].binding = 0;
  attributeDescriptions[0].location = 0;
  attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[0].offset = offsetof(Vertex, position);

  attributeDescriptions[1].binding = 0;
  attributeDescriptions[1].location = 1;
  attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[1].offset = offsetof(Vertex, color);
}

/* COMMONLY USED FORMATS
*  float: VK_FORMAT_R32_SFLOAT
*  vec2: VK_FORMAT_R32G32_SFLOAT
*  vec3: VK_FORMAT_R32G32B32_SFLOAT
*  vec4: VK_FORMAT_R32G32B32A32_SFLOAT
*/  

// ------------------------ //
// ---- FILL VERTICIES ---- //
// This is actually a bad way to do this, but it's just for testing
void fillVertices(Vertex *vertex)
{
  float vertices[] = {
    -0.5f, -0.5f, 0.0f, // bottom left
    0.5f, -0.5f, 0.0f, // bottom right
    0.0f, 0.5f, 0.0f   // top
  };

  float colors[] = {
    1.0f, 0.0f, 1.0f, // bottom left
    0.0f, 1.0f, 0.0f, // bottom right
    0.0f, 0.0f, 1.0f  // top
  };

  for(int i = 0; i < 3; i++)
  {
    vertex[i].position.x = vertices[i*3];
    vertex[i].position.y = vertices[i*3+1];
    vertex[i].color.x = colors[i*3]; 
    vertex[i].color.y = colors[i*3+1]; 
    vertex[i].color.z = colors[i*3+2]; 
  }
}

// ------------------------ //
// --- FIND MEMORY TYPE --- //
u32 findMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties, App *pApp)
{
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(pApp->physicalDevice, &memProperties);

  for(u32 i = 0; i < memProperties.memoryTypeCount; i++)
  {
    if(typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
      return i;
  }

  printf("Failed to find suitable memory type\n");
  return EXIT_FAILURE;
}

// ------------------------ //
// ---- CREATE BUFFER ----- //
void createVertexBuffer(App *pApp)
{
  Vertex *vertices; 
  vertices = malloc(sizeof(Vertex)*3);
  fillVertices(vertices);
  
  VkBufferCreateInfo bufferInfo = {0};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = sizeof(vertices[0])*3;
  bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if(vkCreateBuffer(pApp->logicalDevice, &bufferInfo, NULL, &pApp->vertexBuffer) != VK_SUCCESS)
  {
    printf("Failed to create vertex buffer\n");
    exit(1);
  }

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(pApp->logicalDevice, pApp->vertexBuffer, &memRequirements);

  VkMemoryAllocateInfo allocInfo = {
    .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    .allocationSize = memRequirements.size,
    .memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pApp)};
  if(vkAllocateMemory(pApp->logicalDevice, &allocInfo, NULL, &pApp->vertexBufferMemory) != VK_SUCCESS)
  {
    printf("Failed to allocate vertex buffer memory\n");
    exit(1);
  }

  vkBindBufferMemory(pApp->logicalDevice, pApp->vertexBuffer, pApp->vertexBufferMemory, 0);

  void *data;
  vkMapMemory(pApp->logicalDevice, pApp->vertexBufferMemory, 0, bufferInfo.size, 0, &data);
  memcpy(data, vertices, bufferInfo.size);
  vkUnmapMemory(pApp->logicalDevice, pApp->vertexBufferMemory);
  free(vertices);
}

// ------------------------ //
// ---- CLEANUP BUFFER ---- // 
void cleanupVertexBuffer(App *pApp)
{
  vkDestroyBuffer(pApp->logicalDevice, pApp->vertexBuffer, NULL);
  vkFreeMemory(pApp->logicalDevice, pApp->vertexBufferMemory, NULL);
}
