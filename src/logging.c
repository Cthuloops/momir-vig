#include "logging.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define TIME_SIZE 9UL

static enum LogLevel current_log_level = MOMIR_LOG_INFO;
static const char *log_str[] = { "NONE", "DEBUG", "INFO", "ERROR", "FATAL" };
static FILE *log_out_file = NULL;

static FILE *
get_stream (void)
{
  return log_out_file ? log_out_file : stderr;
}

void
log_init (const char *out_path)
{
  if (out_path)
    {
      log_out_file = fopen (out_path, "a");
      if (log_out_file)
        {
          return;
        }
      fprintf (stderr,
               "Error %i fopen in log_init: %s, defaulting to stderr\n", errno,
               strerror (errno));
    }
  log_out_file = stderr;
}

void
log_set_level (enum LogLevel level)
{
  current_log_level = level;
}

const char *
log_get_level_str (void)
{
  return log_str[current_log_level];
}

enum LogLevel
log_get_level (void)
{
  return current_log_level;
}

void
log_write (enum LogLevel level, const char *module, const char *file,
           const char *function, size_t line, const char *fmt, ...)
{
  if (level < current_log_level || current_log_level == MOMIR_LOG_NONE)
    {
      return;
    }
  FILE *stream = get_stream ();

  char time_buffer[TIME_SIZE];
  const char *current_time = "STRFTIME";

  if (strftime (time_buffer, TIME_SIZE, "%H:%M:%S",
                localtime (&(time_t){ time (NULL) }))
      != 0)
    {
      current_time = time_buffer;
    }
  else
    {
      fprintf (stderr,
               "Error strftime: exceeded %zu characters available for"
               " formatting\n",
               TIME_SIZE);
    }

  fprintf (stream, "%s [%s] %s %s (%s:%zu):", log_str[level],
           module ? module : "", current_time, function, file, line);

  va_list args;
  va_start (args, fmt);
  vfprintf (stream, fmt, args);
  va_end (args);

  fputc ('\n', stream);

  fflush (stream);
}

void
log_deinit (void)
{
  if (!log_out_file || log_out_file == stderr || log_out_file == stdout)
    {
      return;
    }

  fclose (log_out_file);
  log_out_file = NULL;
}
