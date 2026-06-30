#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "constants.h"
#include "http_client.h"
#include "logging.h"
#include "utils.h"

void
write_to_file (const struct CurlFatStruct *cfs, const char *filename)
{
  if (!cfs || !cfs->memory || cfs->size == EXIT_SUCCESS)
    {
      LOG_ERROR (MOMIR_MAIN, "Error write_to_file: Invalid or empty buffer\n");
      return;
    }

  FILE *fp = fopen (filename, "w");
  if (!fp)
    {
      LOG_ERROR (MOMIR_MAIN, "Error %i from fopen: %s\n", errno,
                 strerror (errno));
      return;
    }

  size_t written = fwrite (cfs->memory, sizeof (char), cfs->size, fp);

  if (written < cfs->size)
    {
      LOG_ERROR (MOMIR_MAIN, "Error fwrite: %zu of %zu bytes written.\n",
                 written, cfs->size);
    }

  fclose (fp);
}

int
main (void)
{
  int out_status = EXIT_FAILURE;
  log_init ("./log.log");

  log_set_level (MOMIR_LOG_DEBUG);

  struct CurlFatStruct cfs;
  memset (&cfs, 0, sizeof (cfs));
  if (!curl_init (&cfs))
    {
      LOG_FATAL (MOMIR_MAIN, "curl failed to init");
      goto cleanup_log;
    }

  // Make sure our data directory exists
  if (!file_exists (MOMIR_DATA_DIR, true, true))
    {
      LOG_FATAL (MOMIR_MAIN, "Error: Couldn't ensure data directory exists");
      goto cleanup_curl;
    }

  if (!file_exists (MOMIR_BULK_DATA_PATH, false, false))
    {
      if (scryfall_bulk_data (&cfs))
        {
          write_to_file (&cfs, MOMIR_BULK_DATA_PATH);
        }
      else
        {
          LOG_FATAL (MOMIR_MAIN, "Error: scryfall call failed");
          goto cleanup_curl;
        }
    }
  else
    {
      if (!bulk_file_needs_update (MOMIR_BULK_DATA_PATH))
        {
          LOG_INFO (MOMIR_MAIN, "Bulk data is up-to-date");
          out_status = EXIT_SUCCESS;
          goto cleanup_curl;
        }
    }

cleanup_curl:
  curl_deinit (&cfs);

cleanup_log:
  log_deinit ();

  return out_status;
}
