#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "http_client.h"

#define BULK_DATA_URL "https://api.scryfall.com/bulk-data"
#define USER_AGENT "User-Agent: MomirVigApp/0.1"
#define ACCEPT "Accept: */*"

static size_t
write_cb (void *contents, size_t size, size_t memb, void *userp)
{
  size_t realsize = size * memb;

  struct CurlFatStruct *mem = (struct CurlFatStruct *)userp;

  char *ptr = realloc (mem->memory, mem->size + realsize + 1);
  if (!ptr)
    {
      fprintf (stderr, "Error %i from write_cb: %s\n", errno,
               strerror (errno));
      return 0;
    }

  mem->memory = ptr;
  memcpy (&(mem->memory[mem->size - 1]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = '\0';

  return realsize;
}

int
curl_init (struct CurlFatStruct *cfs)
{
  int out_status = 1;

  if (!cfs)
    {
      fprintf (stderr, "Passed NULL to curl_init\n");
      goto out;
    }

  CURLcode result = curl_global_init (CURL_GLOBAL_ALL);
  if (result)
    {
      fprintf (stderr, "Failed to initialize curl: %s\n",
               curl_easy_strerror (result));
      out_status = (int)result;
      goto out;
    }

  cfs->memory = malloc (1);
  if (!cfs->memory)
    {
      fprintf (stderr, "Error %i from malloc: %s\n", errno, strerror (errno));
      out_status = errno;
      goto out;
    }
  cfs->size = 1;

  cfs->curl = curl_easy_init ();
  if (!cfs->curl)
    {
      fprintf (stderr, "Something went wrong with curl_easy_init()\n");
      goto cleanup_memory;
    }

  // Write the information from requests into the struct.
  curl_easy_setopt (cfs->curl, CURLOPT_WRITEFUNCTION, write_cb);
  curl_easy_setopt (cfs->curl, CURLOPT_WRITEDATA, (void *)cfs);

  cfs->list = NULL;

  // Attach the required headers.
  cfs->list = curl_slist_append (cfs->list, USER_AGENT);
  cfs->list = curl_slist_append (cfs->list, ACCEPT);
  curl_easy_setopt (cfs->curl, CURLOPT_HTTPHEADER, cfs->list);

  goto out;

cleanup_memory:
  free (cfs->memory);

out:
  return out_status;
}

int
scryfall_bulk_data (struct CurlFatStruct *cfs)
{
  int out_status = -1;
  if (!cfs)
    {
      fprintf (stderr, "Passed NULL to scryfall_bulk_data()\n");
      goto out;
    }

  if (!cfs->curl)
    {
      fprintf (stderr, "Curl is not intialized in scryfall_bulk_data()\n");
      goto out;
    }

  if (!cfs->memory)
    {
      fprintf (stderr, "Memory is not initialized in scryfall_bulk_data()\n");
      goto out;
    }

  curl_easy_setopt (cfs->curl, CURLOPT_URL, BULK_DATA_URL);

  CURLcode result = curl_easy_perform (cfs->curl);
  if (result != CURLE_OK)
    {
      fprintf (stderr, "curl_easy_perform() failed: %s\n",
               curl_easy_strerror (result));
      out_status = (int)result;
      goto out;
    }

  out_status = 1;

out:
  return out_status;
}

void
curl_deinit (struct CurlFatStruct *cfs)
{
  if (!cfs)
    {
      goto out;
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

out:
  return;
}
