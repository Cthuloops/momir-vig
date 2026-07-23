#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#include "bulk_data.h"
#include "constants.h"
#include "logging.h"
#include "utils.h"

struct BulkDataItem *
bulk_item_from_file (const char *file_path)
{
  struct BulkDataItem *item = NULL;

  if (!file_path)
    {
      LOG_ERROR (MOMIR_BULK_DATA,
                 "Passed NULL file path to bulk_item_from_file");
      goto out;
    }

  if (strcmp (file_path, "") == 0)
    {
      LOG_ERROR (MOMIR_BULK_DATA,
                 "Passed empty file path to bulk_item_from_file");
      goto out;
    }

  if (!file_exists (file_path, false))
    {
      LOG_ERROR (MOMIR_BULK_DATA, "File %s does not exist");
      goto out;
    }

  struct stat stats;
  if (stat (file_path, &stats) == 0)
    {
      if (!S_ISREG (stats.st_mode))
        {
          LOG_ERROR (MOMIR_BULK_DATA, "File %s is not a regular file.");
          goto out;
        }
    }
  else
    {
      LOG_ERROR (MOMIR_BULK_DATA, "Error from stat: %s", strerror (errno));
      goto out;
    }

  char *bulk_item_string = malloc (stats.st_size + 1);
  if (!bulk_item_string)
    {
      LOG_ERROR (MOMIR_BULK_DATA, "Failed to allocate %zu bytes for file %s",
                 stats.st_size + 1, file_path);
      goto out;
    }

  item = malloc (sizeof (struct BulkDataItem));
  if (!item)
    {
      LOG_ERROR (MOMIR_BULK_DATA,
                 "Failed to allocate BulkDataItem in bulk_item_from_file");
      goto cleanup_bulk_string;
    }

cleanup_bulk_string:
  if (bulk_item_string)
    {
      free (bulk_item_string);
    }

out:
  return item;
}
