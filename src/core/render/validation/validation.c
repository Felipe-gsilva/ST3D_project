#include "validation.h"

bool checkValidationLayerSupport()
{
  u32 layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, NULL);

  VkLayerProperties *availableLayers = malloc(sizeof(VkLayerProperties) * layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

  printf("available layers:\n");
  for (u32 i = 0; i < layerCount; i++)
  {
    printf("\t%s\n", availableLayers[i].layerName);
  }

  validate();
  free(availableLayers);
  return true;
}

void validate()
{
  // todo check which layers are available
  printf("validate\n");
}