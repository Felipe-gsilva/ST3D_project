#include "logger.h"

bool initLogging(App *app)
{
  app->logger = malloc(sizeof(Logger));
  app->logger->logLevel = LOG_LEVEL_INFO;
  app->logger->file = fopen("log.txt", "w");
  if (app->logger->file == NULL)
  {
    printf("Failed to open log file!\n");
    return false;
  }
  return true;
}

void shutdownLogger(App *app)
{
  fclose(app->logger->file);
  free(app->logger);
}
