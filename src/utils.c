#define _XOPEN_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "../vendor/cJSON.h"

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

bool
bulk_file_needs_update (const char *file_path)
{
  LOG_DEBUG (MOMIR_UTILS, "Checking if bulk file needs update");

  if (!file_path || strcmp (file_path, "") == 0)
    {
      LOG_ERROR (MOMIR_UTILS,
                 "Passed NULL or empty string to bulk_file_needs_update");
      return false;
    }

  if (!file_exists (file_path, false))
    {
      LOG_ERROR (MOMIR_UTILS, "File %s does not exist");
      return false;
    }

  struct stat stats;
  if (stat (file_path, &stats) == 0)
    {
      if (!S_ISREG (stats.st_mode))
        {
          LOG_ERROR (MOMIR_UTILS, "Path type mismatch for %s (Expected file)",
                     file_path);
          return false;
        }
    }
  else
    {
      LOG_ERROR (MOMIR_UTILS, "Error from stat at %s: %s", file_path,
                 strerror (errno));
      return false;
    }

  bool out_status = false;

  const size_t bulk_string_size = stats.st_size + 1;
  char *bulk_string = malloc (bulk_string_size);
  if (!bulk_string)
    {
      LOG_ERROR (MOMIR_UTILS, "Failed to allocate %zu bytes for %s file",
                 bulk_string_size, file_path);
      goto cleanup_bulk_string;
    }

  FILE *fp = fopen (file_path, "r");
  if (!fp)
    {
      LOG_ERROR (MOMIR_UTILS, "Failed to open file %s", file_path);
      goto cleanup_bulk_file;
    }

  fgets (bulk_string, bulk_string_size, fp);
  bulk_string[stats.st_size] = '\0';

  cJSON *bulk_json = cJSON_Parse (bulk_string);
  if (!bulk_json)
    {
      const char *err = cJSON_GetErrorPtr ();
      if (err)
        {
          LOG_ERROR (MOMIR_UTILS, "Error cJSON_Parse: %s", err);
          goto cleanup_cjson;
        }
    }

  const cJSON *data = cJSON_GetObjectItem (bulk_json, "data");
  if (!data)
    {
      LOG_ERROR (MOMIR_UTILS, "Item 'data' not found in %s", file_path);
      goto cleanup_cjson;
    }

  // Search for the default card object and check the last time it was updated.
  struct tm last_updated_calender = { 0 };
  const cJSON *datum;
  cJSON_ArrayForEach (datum, data)
  {
    const cJSON *type = cJSON_GetObjectItem (datum, "type");
    if (!type)
      {
        LOG_ERROR (MOMIR_UTILS, "Item 'type' not found in data");
        goto cleanup_cjson;
      }
    if (cJSON_IsString (type) && strcmp (type->valuestring, "default_cards"))
      {
        const cJSON *updated_at = cJSON_GetObjectItem (datum, "updated_at");
        if (!updated_at)
          {
            LOG_ERROR (MOMIR_UTILS, "Item 'updated_at' not found");
            goto cleanup_cjson;
          }

        if (cJSON_IsString (updated_at))
          {
            // ignore the return, we only yyyy-mm-dd.
            strptime (updated_at->valuestring, "%Y-%m-%d",
                      &last_updated_calender);
            if (difftime (time (NULL), mktime (&last_updated_calender))
                >= SCRYFALL_BULK_DATA_UPDATE_TIME)
              {
                LOG_DEBUG (MOMIR_UTILS,
                           "Default cards last updated at %s, needs updating",
                           updated_at->valuestring);
                out_status = true;
                goto cleanup_cjson;
              }
            else
              {
                LOG_DEBUG (
                    MOMIR_UTILS,
                    "Default cards last updated at %s, no need to update",
                    updated_at->valuestring);
              }

            goto cleanup_cjson;
          }
      }
  }

cleanup_cjson:
  cJSON_Delete (bulk_json);

cleanup_bulk_file:
  if (fp)
    {
      fclose (fp);
    }

cleanup_bulk_string:
  if (bulk_string)
    {
      free (bulk_string);
    }

  return out_status;
}

bool
bulk_data_item_up_to_date (const struct BulkDataItem *bdi)
{
  return difftime (time (NULL), bdi->updated_at)
         < SCRYFALL_BULK_DATA_UPDATE_TIME;
}
