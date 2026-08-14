#define _XOPEN_SOURCE 700 // for strptime

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "../vendor/cJSON/cJSON.h"

#include "bulk_data.h"
#include "constants.h"
#include "guard.h"
#include "logging.h"
#include "utils.h"

static void
fill_bulk_item_type (struct BulkDataItem *bdi, const cJSON *item)
{
  const cJSON *type_name = cJSON_GetObjectItemCaseSensitive (item, "type");
  if (cJSON_IsString (type_name) && (type_name->valuestring))
    {
      size_t type_name_length = strlen (type_name->valuestring);
      bdi->type_name = malloc (type_name_length + 1);
      if (!bdi->type_name)
        {
          LOG_ERROR (MOMIR_BULK_DATA, "Failed to allocate %zu bytes for %s",
                     type_name_length + 1, type_name->valuestring);
          return;
        }

      strncpy (bdi->type_name, type_name->valuestring, type_name_length);
      bdi->type_name[type_name_length] = '\0';

      int i = 0;
      while (i < BulkDataItem_str_size)
        {
          if (strcmp (bdi->type_name, BulkDataItem_str[i]) == 0)
            {
              bdi->type = i;
              break;
            }
        }

      // Set the type to Uknown.
      if (i == BulkDataItem_str_size)
        {
          bdi->type = BulkDataItem_str_size;
        }
    }
}

static void
fill_bulk_item_updated_at (struct BulkDataItem *bdi, const cJSON *item)
{
  const cJSON *updated_at
      = cJSON_GetObjectItemCaseSensitive (item, "updated_at");
  if (cJSON_IsString (updated_at) && (updated_at->valuestring))
    {
      struct tm json_tm = { 0 };
      const char *end = strptime (updated_at->valuestring, "%FT%T", &json_tm);
      if (*end != '.')
        {
          LOG_ERROR (
              MOMIR_BULK_DATA,
              "Expected strptime to stop parsing at '.'; got '%c' instead.",
              *end);
        }

      bdi->updated_at = mktime (&json_tm);
    }
}

static void
fill_bulk_item_jsonl_size (struct BulkDataItem *bdi, const cJSON *item)
{
  const cJSON *jsonl_size
      = cJSON_GetObjectItemCaseSensitive (item, "compressed_size");
  if (cJSON_IsNumber (jsonl_size))
    {
      bdi->jsonl_size = (size_t)jsonl_size->valueint;
    }
}

static void
fill_bulk_item_uri (struct BulkDataItem *bdi, const cJSON *item)
{
  const cJSON *uri = cJSON_GetObjectItemCaseSensitive (item, "uri");
  if (cJSON_IsString (uri) && (uri->valuestring))
    {
      size_t uri_length = strlen (uri->valuestring);
      bdi->uri = malloc (uri_length + 1);
      if (!bdi->uri)
        {
          LOG_ERROR (MOMIR_BULK_DATA, "Failed to allocate %zu bytes for %s",
                     uri_length + 1, uri->valuestring);
          return;
        }

      strncpy (bdi->uri, uri->valuestring, uri_length);
      bdi->uri[uri_length] = '\0';
    }
}

static void
fill_bulk_item_jsonl_uri (struct BulkDataItem *bdi, const cJSON *item)
{
  const cJSON *jsonl_uri = cJSON_GetObjectItemCaseSensitive (item, "uri");
  if (cJSON_IsString (jsonl_uri) && (jsonl_uri->valuestring))
    {
      size_t jsonl_uri_length = strlen (jsonl_uri->valuestring) + 1;
      bdi->jsonl_download_uri = malloc (jsonl_uri_length);
      if (!bdi->jsonl_download_uri)
        {
          LOG_ERROR (MOMIR_BULK_DATA, "Failed to allocate %zu bytes for %s",
                     jsonl_uri_length + 1, jsonl_uri->valuestring);
          return;
        }

      strncpy (bdi->uri, jsonl_uri->valuestring, jsonl_uri_length);
      bdi->uri[jsonl_uri_length] = '\0';
    }
}

static void
fill_bulk_item_struct (struct BulkDataItem *bdi, const char *json_str)
{
  cJSON *json = cJSON_Parse (json_str);
  if (!json)
    {
      const char *error_ptr = cJSON_GetErrorPtr ();
      if (error_ptr)
        {
          LOG_ERROR (MOMIR_BULK_DATA, "cJSON parse error before %s",
                     error_ptr);
          return;
        }
    }

  fill_bulk_item_type (bdi, json);
  if (!bdi->type_name)
    {
      LOG_ERROR (MOMIR_BULK_DATA, "No type available for bulk item (%s)",
                 json);
      goto cleanup;
    }

  fill_bulk_item_updated_at (bdi, json);
  if (bdi->updated_at == (time_t)-1)
    {
      LOG_ERROR (MOMIR_BULK_DATA, "Error %s from mktime (%s)",
                 strerror (errno), json);
      goto cleanup;
    }

  fill_bulk_item_jsonl_size (bdi, json);

  fill_bulk_item_uri (bdi, json);
  if (!bdi->uri || strcmp (bdi->uri, "") == 0)
    {
      LOG_ERROR (MOMIR_BULK_DATA, "uri is null or empty (%s)", json);
      goto cleanup;
    }

  fill_bulk_item_jsonl_uri (bdi, json);
  if (!bdi->jsonl_download_uri || strcmp (bdi->jsonl_download_uri, "") == 0)
    {
      LOG_ERROR (MOMIR_BULK_DATA, "jsonl_download_uri is null or empty (%s)",
                 json);
    }

cleanup:
  cJSON_Delete (json);
}

struct BulkDataItem *
bulk_item_from_string (const char *json_str)
{
  struct BulkDataItem *item = NULL;
  PARAMETER_STRING_GUARD (json_str, MOMIR_BULK_DATA, out);

  item = malloc (sizeof (struct BulkDataItem));
  if (!item)
    {
      LOG_ERROR (MOMIR_BULK_DATA, "Failed to allocate BulkDataItem");
      goto out;
    }

  fill_bulk_item_struct (item, json_str);

out:
  return item;
}

struct BulkDataItem *
bulk_item_from_file (const char *file_path)
{
  struct BulkDataItem *item = NULL;

  PARAMETER_STRING_GUARD (file_path, MOMIR_BULK_DATA, out);

  if (!file_exists (file_path, false))
    {
      LOG_ERROR (MOMIR_BULK_DATA, "File %s does not exist", file_path);
      goto out;
    }

  struct stat stats;
  if (stat (file_path, &stats) == 0)
    {
      if (!S_ISREG (stats.st_mode))
        {
          LOG_ERROR (MOMIR_BULK_DATA, "File %s is not a regular file.",
                     file_path);
          goto out;
        }
    }
  else
    {
      LOG_ERROR (MOMIR_BULK_DATA, "Stat failed with error %s",
                 strerror (errno));
      goto out;
    }

  const size_t bulk_size = stats.st_size + 1;
  char *bulk_item_string = malloc (bulk_size);
  if (!bulk_item_string)
    {
      LOG_ERROR (MOMIR_BULK_DATA, "Failed to allocate %zu bytes for file %s",
                 stats.st_size + 1, file_path);
      goto out;
    }

  FILE *fp = fopen (file_path, "rb");
  if (!fp)
    {
      LOG_ERROR (MOMIR_BULK_DATA, "fopen failed: %zu", errno);
      goto cleanup_bulk_string;
    }

  size_t ret = fread (bulk_item_string, sizeof (char), bulk_size, fp);
  if (ret != bulk_size)
    {
      LOG_ERROR (MOMIR_BULK_DATA, "fread failed: %zu", ret);
      goto cleanup_fopen;
    }

  item = bulk_item_from_string (bulk_item_string);

cleanup_fopen:
  fclose (fp);

cleanup_bulk_string:
  free (bulk_item_string);

out:
  return item;
}

void
bulk_item_free (struct BulkDataItem *bdi)
{
  PARAMETER_NULL_GUARD (bdi, MOMIR_BULK_DATA, out);

  free (bdi->jsonl_download_uri);
  free (bdi->uri);
  free (bdi->type_name);
  free (bdi);

out:
  bdi = NULL;
}
