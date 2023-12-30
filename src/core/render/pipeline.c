#include "pipeline.h"

void createImageViews(App *pApp)
{
  pApp->swapChainImageViews = malloc(sizeof(VkImageView) * pApp->swapChainImageCount);

  for (size_t i = 0; i < pApp->swapChainImageCount; i++)
  {
    VkImageViewCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = pApp->swapChainImages[i],
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = pApp->swapChainImageFormat,
        .components.r = VK_COMPONENT_SWIZZLE_IDENTITY,
        .components.g = VK_COMPONENT_SWIZZLE_IDENTITY,
        .components.b = VK_COMPONENT_SWIZZLE_IDENTITY,
        .components.a = VK_COMPONENT_SWIZZLE_IDENTITY,
        .subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .subresourceRange.baseMipLevel = 0,
        .subresourceRange.levelCount = 1,
        .subresourceRange.baseArrayLayer = 0,
        .subresourceRange.layerCount = 1};

    if (vkCreateImageView(pApp->device->logicalDevice, &createInfo, NULL, &pApp->swapChainImageViews[i]) != VK_SUCCESS)
    {
      puts("failed to create image views!");
      exit(EXIT_FAILURE);
    }
  }
}

void readFile(const char *filename, ShaderFile *shader)
{
  FILE *pFile;

  pFile = fopen(filename, "rb");
  if (pFile == NULL)
  {
    printf("Failed to open %s\n", filename);
    exit(7);
  }

  fseek(pFile, 0L, SEEK_END);
  shader->size = ftell(pFile);

  fseek(pFile, 0L, SEEK_SET);

  shader->code = (char *)malloc(sizeof(char) * shader->size);
  size_t readCount = fread(shader->code, shader->size, sizeof(char), pFile);
  printf("ReadCount: %ld\n", readCount);

  fclose(pFile);
}

VkShaderModule createShaderModule(App *pApp, ShaderFile *shaderFile)
{
  VkShaderModuleCreateInfo createInfo = {
      .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
      .codeSize = shaderFile->size,
      .pCode = (u32 *)shaderFile->code};

  VkShaderModule shaderModule;
  if (vkCreateShaderModule(pApp->device->logicalDevice, &createInfo, NULL, &shaderModule) != VK_SUCCESS)
  {
    puts("failed to create shader module!");
    exit(EXIT_FAILURE);
  }
  return shaderModule;
}

void createGraphicsPipeline(App *pApp)
{
  ShaderFile vertShader = {0};
  ShaderFile fragShader = {0};
  readFile("shaders/vertex_shader.vert.spv", &vertShader);
  readFile("shaders/fragment_shader.frag.spv", &fragShader);

  VkShaderModule vertShaderModule = createShaderModule(pApp, &vertShader);
  VkShaderModule fragShaderModule = createShaderModule(pApp, &fragShader);

  VkPipelineShaderStageCreateInfo vertShaderStageInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_VERTEX_BIT,
      .module = vertShaderModule,
      .pName = "main"};
  VkPipelineShaderStageCreateInfo fragShaderStageInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
      .module = fragShaderModule,
      .pName = "main"};

  pApp->shaderCount = 2;
  pApp->shaderModules = malloc(sizeof(VkShaderModule) * pApp->shaderCount);
  pApp->shaderModules[0] = vertShaderModule;
  pApp->shaderModules[1] = fragShaderModule;
  pApp->shaderStages = {vertShaderStageInfo, fragShaderStageInfo};    
}

void createGraphicsQueue()
{
  printf("create graphics queue\n");
}

void cleanupImageViews(App *pApp)
{
  for (size_t i = 0; i < pApp->swapChainImageCount; i++)
  {
    vkDestroyImageView(pApp->device->logicalDevice, pApp->swapChainImageViews[i], NULL);
  }
}

void cleanupShaders(App *pApp)
{
  vkDestroyShaderModule(pApp->device->logicalDevice, pApp->shaderModules[0], NULL);
  vkDestroyShaderModule(pApp->device->logicalDevice, pApp->shaderModules[1], NULL);
  free(pApp->shaderModules);
}