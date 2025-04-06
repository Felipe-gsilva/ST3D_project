#pragma once

#include "defines.h"

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

#if REALEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

typedef enum log_level {
  LOG_LEVEL_FATAL = 0,
  LOG_LEVEL_ERROR = 1,
  LOG_LEVEL_WARN = 2,
  LOG_LEVEL_INFO = 3,
  LOG_LEVEL_DEBUG = 4,
  LOG_LEVEL_TRACE = 5,
} log_level;

bool initLogging(App *app);
void shutdownLogging(App *app);

void logMessage(log_level level, const char *message, ...);

// log a fatal message
#define LOG_FATAL(message, ...)                                                \
  logMessage(LOG_LEVEL_FATAL, message, ##__VA_ARGS__);

// log an error message
#ifndef LOG_ERROR
#define LOG_ERROR(message, ...)                                                \
  logMessage(LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#endif

// log a warning message
#if LOG_WARN_ENABLED == 1
#define LOG_WARN(message, ...)                                                 \
  logMessage(LOG_LEVEL_WARN, message, ##__VA_ARGS__);
#else
#define LOG_WARN(message, ...)
#endif

// log an info message
#if LOG_INFO_ENABLED == 1
#define LOG_INFO(message, ...)                                                 \
  logMessage(LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#else
#define LOG_INFO(message, ...)
#endif

// log a debug message
#if LOG_DEBUG_ENABLED == 1
#define LOG_DEBUG(message, ...)                                                \
  logMessage(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#else
#define LOG_DEBUG(message, ...)
#endif

// log a trace message
#if LOG_TRACE_ENABLED == 1
#define LOG_TRACE(message, ...)                                                \
  logMessage(LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else
#define LOG_TRACE(message, ...)
#endif
