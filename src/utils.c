#define _XOPEN_SOURCE 700

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "../vendor/cJSON/cJSON.h"

#include "constants.h"
#include "logging.h"
#include "utils.h"

static bool
create_file (const char *file_path, bool is_dir)
{
  if (!file_path || (strcmp (file_path, "") == 0))
    {
      LOG_ERROR (MOMIR_UTILS, "Passed NULL or empty file_path to create_file");
      return false;
    }

  if (is_dir)
    {
      if (mkdir (file_path, 0755) == -1)
        {
          LOG_ERROR (MOMIR_UTILS, "Error from mkdir at %s: %s", file_path,
                     strerror (errno));
          return false;
        }
      return true;
    }

  int fd = open (file_path, O_WRONLY | O_CREAT | O_EXCL, 0644);
  if (fd == -1)
    {
      if (errno == EEXIST)
        {
          return true;
        }
      else
        {
          LOG_ERROR (MOMIR_UTILS, "Error creating file at %s: %s", file_path,
                     strerror (errno));
          return false;
        }
    }

  if (close (fd) == -1)
    {
      LOG_ERROR (MOMIR_UTILS, "Error closing file %s: %s", file_path,
                 strerror (errno));
    }
  return true;
}

bool
file_exists (const char *file_path, bool is_dir)
{
  if (!file_path)
    {
      LOG_ERROR (MOMIR_UTILS, "Passed NULL path to file_exists");
      return false;
    }

  if (strcmp (file_path, "") == 0)
    {
      LOG_ERROR (MOMIR_UTILS, "Passed empty string for path to file_exists");
      return false;
    }

  struct stat stats;

  LOG_DEBUG (MOMIR_UTILS, "Looking for %s at: %s",
             is_dir ? "directory" : "file", file_path);
  if (stat (file_path, &stats) == 0)
    {
      if (S_ISDIR (stats.st_mode) != is_dir)
        {
          LOG_ERROR (MOMIR_UTILS, "Path type mismatch for %s (Expected %s)",
                     file_path, is_dir ? "directory" : "file");
          return false;
        }
      return true;
    }
  else
    {
      LOG_ERROR (MOMIR_UTILS, "Error from stat at %s: %s", file_path,
                 strerror (errno));
      return false;
    }

  return false;
}

bool
create_dir (const char *dir_path)
{
  if (!dir_path)
    {
      LOG_ERROR (MOMIR_UTILS, "Passed NULL dir_path to create_dir");
      return false;
    }

  if (file_exists (dir_path, true))
    {
      return true;
    }
  else
    {
      return create_file (dir_path, true);
    }
  return false;
}
