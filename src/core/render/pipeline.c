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

    if (vkCreateImageView(pApp->logicalDevice, &createInfo, NULL, &pApp->swapChainImageViews[i]) != VK_SUCCESS)
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
  if (vkCreateShaderModule(pApp->logicalDevice, &createInfo, NULL, &shaderModule) != VK_SUCCESS)
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
  pApp->shaderStages = malloc(sizeof(VkPipelineShaderStageCreateInfo) * pApp->shaderCount);
  pApp->shaderStages[0] = vertShaderStageInfo;
  pApp->shaderStages[1] = fragShaderStageInfo;

  VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
      .vertexBindingDescriptionCount = 0,
      .pVertexBindingDescriptions = NULL, // Optional
      .vertexAttributeDescriptionCount = 0,
      .pVertexAttributeDescriptions = NULL};

  VkPipelineInputAssemblyStateCreateInfo inputAssembly = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
      .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
      .primitiveRestartEnable = VK_FALSE};

  VkDynamicState dynamicStates[] = {
      VK_DYNAMIC_STATE_VIEWPORT,
      VK_DYNAMIC_STATE_LINE_WIDTH,
      VK_DYNAMIC_STATE_SCISSOR};

  size_t dynamicStateCount = sizeof(dynamicStates) / sizeof(dynamicStates[0]);

  VkPipelineDynamicStateCreateInfo dynamicState = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
      .dynamicStateCount = dynamicStateCount,
      .pDynamicStates = dynamicStates};

  VkPipelineViewportStateCreateInfo viewportState = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
      .viewportCount = 1,
      .scissorCount = 1};

  VkPipelineRasterizationStateCreateInfo rasterizer = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
      .depthClampEnable = VK_FALSE,
      .rasterizerDiscardEnable = VK_FALSE,
      .polygonMode = VK_POLYGON_MODE_FILL,
      .lineWidth = 1.0f,
      .cullMode = VK_CULL_MODE_BACK_BIT,
      .frontFace = VK_FRONT_FACE_CLOCKWISE,
      .depthBiasEnable = VK_FALSE,
      .depthBiasConstantFactor = 0.0f,
      .depthBiasClamp = 0.0f,
      .depthBiasSlopeFactor = 0.0f};

  VkPipelineMultisampleStateCreateInfo multisampling = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
      .sampleShadingEnable = VK_FALSE,
      .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
      .minSampleShading = 1.0f,
      .pSampleMask = NULL,
      .alphaToCoverageEnable = VK_FALSE,
      .alphaToOneEnable = VK_FALSE};

  VkPipelineColorBlendAttachmentState colorBlendAttachment = {
      .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
      .blendEnable = VK_FALSE,
      .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
      .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
      .colorBlendOp = VK_BLEND_OP_ADD,
      .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
      .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
      .alphaBlendOp = VK_BLEND_OP_ADD};

  VkPipelineColorBlendStateCreateInfo colorBlending = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
      .logicOpEnable = VK_FALSE,
      .logicOp = VK_LOGIC_OP_COPY,
      .attachmentCount = 1,
      .pAttachments = &colorBlendAttachment,
      .blendConstants[0] = 0.0f,
      .blendConstants[1] = 0.0f,
      .blendConstants[2] = 0.0f,
      .blendConstants[3] = 0.0f};

  VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
      .setLayoutCount = 0,
      .pSetLayouts = NULL,
      .pushConstantRangeCount = 0,
      .pPushConstantRanges = NULL};

  if (vkCreatePipelineLayout(pApp->logicalDevice, &pipelineLayoutInfo, NULL, &pApp->pipelineLayout) != VK_SUCCESS)
  {
    puts("failed to create pipeline layout!");
    exit(EXIT_FAILURE);
  }
  VkAttachmentDescription colorAttachment = {
      .format = pApp->swapChainImageFormat,
      .samples = VK_SAMPLE_COUNT_1_BIT,
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
      .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR};

  VkAttachmentReference colorAttachmentRef = {
      .attachment = 0,
      .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

  VkSubpassDescription subpass = {
      .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
      .colorAttachmentCount = 1,
      .pColorAttachments = &colorAttachmentRef};

  VkRenderPassCreateInfo renderPassInfo = {
      .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
      .attachmentCount = 1,
      .pAttachments = &colorAttachment,
      .subpassCount = 1,
      renderPassInfo.pSubpasses = &subpass};

  if (vkCreateRenderPass(pApp->logicalDevice, &renderPassInfo, NULL, &pApp->renderPass) != VK_SUCCESS)
  {
    puts("failed to create render pass!");
    exit(EXIT_FAILURE);
  }

  VkGraphicsPipelineCreateInfo pipelineInfo = {
      .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
      .stageCount = pApp->shaderCount,
      .pStages = pApp->shaderStages,
      .pVertexInputState = &vertexInputInfo,
      .pInputAssemblyState = &inputAssembly,
      .pViewportState = &viewportState,
      .pRasterizationState = &rasterizer,
      .pMultisampleState = &multisampling,
      .pDepthStencilState = NULL,
      .pColorBlendState = &colorBlending,
      .pDynamicState = &dynamicState,
      .layout = pApp->pipelineLayout,
      .renderPass = pApp->renderPass,
      .subpass = 0,
      .basePipelineHandle = VK_NULL_HANDLE,
      .basePipelineIndex = -1};

  if (vkCreateGraphicsPipelines(pApp->logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &pApp->graphicsPipeline) != VK_SUCCESS)
  {
    puts("failed to create graphics pipeline!");
    exit(EXIT_FAILURE);
  }

  free(vertShader.code);
  free(fragShader.code);
  vkDestroyShaderModule(pApp->logicalDevice, fragShaderModule, NULL);
  vkDestroyShaderModule(pApp->logicalDevice, vertShaderModule, NULL);
}

void cleanupImageViews(App *pApp)
{
  for (size_t i = 0; i < pApp->swapChainImageCount; i++)
  {
    vkDestroyImageView(pApp->logicalDevice, pApp->swapChainImageViews[i], NULL);
  }
}

void cleanupShaders(App *pApp)
{
  for (size_t i = 0; i < pApp->shaderCount; i++)
  {
    vkDestroyShaderModule(pApp->logicalDevice, pApp->shaderStages[i].module, NULL);
  }
  free(pApp->shaderStages);
}