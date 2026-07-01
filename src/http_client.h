#ifndef HTTP_CLIENT_H_
#define HTTP_CLIENT_H_

#include <stdlib.h>
#include <time.h>

#include <curl/curl.h>

struct CurlFatStruct
{
  CURL *curl;
  char *memory;
  struct curl_slist *list;
  size_t size;
};

// {
//   "object": "bulk_data",
//   "id": "e2ef41e3-5778-4bc2-af3f-78eca4dd9c23",
//   "type": "default_cards",
//   "updated_at": "2026-06-30T21:09:15.220+00:00",
//   "uri":
//   "https://api.scryfall.com/bulk-data/e2ef41e3-5778-4bc2-af3f-78eca4dd9c23",
//   "name": "Default Cards",
//   "description": "A JSON file containing every card object on Scryfall in
//   English or the printed language if the card is only available in one
//   language.", "size": 555909537, "download_uri":
//   "https://data.scryfall.io/default-cards/default-cards-20260630210915.json",
//   "content_type": "application/json",
//   "content_encoding": "gzip"
// },

/**
 * Represents a bulk data item from Scryfall API.
 */
struct BulkDataItem
{
  enum
  {
    SCRYFALL_BULK_DEFAULT_CARDS,
  } type;
  time_t updated_at;
  size_t download_size;
  char *uri;
  char *download_uri;
};

/**
 * @brief curl_init initializes curl.
 * @param[in] cfs Pointer to CurlFatStruct instance to manage.
 *
 * Initializes the CurlFatStruct struct to be used for later transfers.
 *
 * @return int 1 if successful.
 */
int curl_init (struct CurlFatStruct *cfs);

/**
 * @brief scryfall_bulk_data issues a request for the bulk_data information.
 * @param[in] cfs Pointer to the CurlFatStruct to use for the request.
 *
 * Downloads the json for the bulk_data api information from scryfall. This
 * gets stored in the CurlFatStruct.
 *
 * @returns int 1 if successul.
 */
int scryfall_bulk_data (struct CurlFatStruct *cfs);

/**
 * @brief curl_deinit deinitializes the CurlFatStruct.
 * @param[in] cfs Pointer to the CurlFatStruct to deinitialize.
 */
void curl_deinit (struct CurlFatStruct *cfs);

#endif // HTTP_CLIENT_H_
