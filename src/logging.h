#ifndef MOMIR_LOGGING_H_
#define MOMIR_LOGGING_H_

#include <stdbool.h>
#include <stdlib.h>

#define LOG_VERSION "0.1.0"

enum LogLevel
{
  MOMIR_LOG_NONE,
  MOMIR_LOG_DEBUG,
  MOMIR_LOG_INFO,
  MOMIR_LOG_ERROR,
  MOMIR_LOG_FATAL
};

void log_set_level (enum LogLevel level);
const char *log_get_level_str (void);
enum LogLevel log_get_level (void);

/**
 * @brief log_init initializes logger.
 * @param[in] out Open the given path for logging, if NULL or path fails,
 * default to stderr
 */
void log_init (const char *out_path);
void log_write (enum LogLevel level, const char *module, const char *file,
                size_t line, const char *fmt, ...);

/**
 * @brief log_deinit deinitializes logging.
 */
void log_deinit (void);

#define LOG_DEBUG(module, fmt, ...)                                           \
  log_write (MOMIR_LOG_DEBUG, module, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define LOG_INFO(module, fmt, ...)                                            \
  log_write (MOMIR_LOG_INFO, module, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define LOG_ERROR(module, fmt, ...)                                           \
  log_write (MOMIR_LOG_ERROR, module, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define LOG_FATAL(module, fmt, ...)                                           \
  log_write (MOMIR_LOG_FATAL, module, __FILE__, __LINE__, fmt, __VA_ARGS__)

#endif // MOMIR_LOGGING_H_
