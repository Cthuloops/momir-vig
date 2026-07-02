#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "constants.h"
#include "http_client.h"
#include "logging.h"

static size_t
write_cb (void *contents, size_t size, size_t memb, void *userp)
{
  size_t realsize = size * memb;

  struct CurlFatStruct *mem = (struct CurlFatStruct *)userp;

  char *ptr = realloc (mem->memory, mem->size + realsize + 1);
  if (!ptr)
    {
      LOG_ERROR (MOMIR_HTTP_CLIENT, "Error %i from write_cb: %s", errno,
                 strerror (errno));
      return realsize;
    }

  mem->memory = ptr;
  memcpy (&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = '\0';

  return realsize;
}

int
curl_init (struct CurlFatStruct *cfs)
{
  int out_status = 1;

  LOG_INFO (MOMIR_HTTP_CLIENT, "Initializing CURL");

  if (!cfs)
    {
      LOG_ERROR (MOMIR_HTTP_CLIENT, "Passed NULL to curl_init");
      goto out;
    }

  CURLcode result = curl_global_init (CURL_GLOBAL_ALL);
  if (result)
    {
      LOG_ERROR (MOMIR_HTTP_CLIENT, "Failed to initialize curl: %s",
                 curl_easy_strerror (result));
      out_status = (int)result;
      goto out;
    }

  cfs->memory = malloc (1);
  if (!cfs->memory)
    {
      LOG_ERROR (MOMIR_HTTP_CLIENT, "Error %i from malloc: %s", errno,
                 strerror (errno));
      out_status = errno;
      goto out;
    }
  cfs->size = 0;

  cfs->curl = curl_easy_init ();
  if (!cfs->curl)
    {
      LOG_ERROR (MOMIR_HTTP_CLIENT,
                 "Something went wrong with curl_easy_init()");
      goto cleanup_memory;
    }

  // Write the information from requests into the struct.
  curl_easy_setopt (cfs->curl, CURLOPT_WRITEFUNCTION, write_cb);
  curl_easy_setopt (cfs->curl, CURLOPT_WRITEDATA, (void *)cfs);

  cfs->list = NULL;

  // Attach the required headers.
  cfs->list = curl_slist_append (cfs->list, SCRYFALL_HEADER_USER_AGENT);
  cfs->list = curl_slist_append (cfs->list, SCRYFALL_ACCEPT);
  curl_easy_setopt (cfs->curl, CURLOPT_HTTPHEADER, cfs->list);

  goto out;

cleanup_memory:
  free (cfs->memory);

out:
  if (out_status == 1)
    {
      LOG_INFO (MOMIR_HTTP_CLIENT, "Successfully intialized CURL");
    }

  return out_status;
}

static int
get_scryfall_data (struct CurlFatStruct *cfs, const char *url)
{
  int out_status = -1;
  if (!cfs)
    {
      LOG_ERROR (MOMIR_HTTP_CLIENT, "Passed NULL cfs to get_scryfall_data");
      goto out;
    }

  if (!url)
    {
      LOG_ERROR (MOMIR_HTTP_CLIENT, "Passed NULL url to get_scryfall_data");
      goto out;
    }

  if (strcmp (url, "") == 0)
    {
      LOG_ERROR (MOMIR_HTTP_CLIENT, "Passed empty url to get_scryfall_data");
      goto out;
    }

  if (!cfs->curl)
    {
      LOG_ERROR (MOMIR_HTTP_CLIENT,
                 "Curl is not intialized in get_scryfall_data");
      goto out;
    }

  if (!cfs->memory)
    {
      LOG_ERROR (MOMIR_HTTP_CLIENT,
                 "Memory is not initialized in get_scryfall_data");
      goto out;
    }

  curl_easy_setopt (cfs->curl, CURLOPT_URL, url);

  LOG_INFO (MOMIR_HTTP_CLIENT, "Issuing get request to %s", url);
  CURLcode result = curl_easy_perform (cfs->curl);
  if (result != CURLE_OK)
    {
      LOG_ERROR (MOMIR_HTTP_CLIENT, "curl_easy_perform() failed: %s",
                 curl_easy_strerror (result));
      out_status = (int)result;
      goto out;
    }

  out_status = 1;

out:
  return out_status;
}

int
get_default_cards (struct CurlFatStruct *cfs)
{
  return get_scryfall_data (cfs, SCRYFALL_DEFAULT_CARDS_URL);
}

void
curl_deinit (struct CurlFatStruct *cfs)
{
  LOG_INFO (MOMIR_HTTP_CLIENT, "Deinitializing CURL");
  if (!cfs)
    {
      LOG_WARN (MOMIR_HTTP_CLIENT, "Passed NULL to curl_deinit");
      return;
    }

  if (cfs->curl)
    {
      curl_easy_cleanup (cfs->curl);
    }

  if (cfs->memory)
    {
      free (cfs->memory);
      cfs->size = 0;
    }

  if (cfs->list)
    {
      curl_slist_free_all (cfs->list);
    }
}
