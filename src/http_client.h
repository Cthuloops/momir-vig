/**
 * @file http_client.h
 * @brief HTTP client functions and macros.
 */
#ifndef HTTP_CLIENT_H_
#define HTTP_CLIENT_H_

#include <stdlib.h>
#include <time.h>

#include <curl/curl.h>

/**
 * @brief Wrapper around CURL handle, memory buffer, and header list.
 */
struct CurlFatStruct
{
  CURL *curl;              /**< CURL easy handle */
  char *memory;            /**< Response buffer */
  struct curl_slist *list; /**< Header list */
  size_t size;             /**< Response buffer size */
};

/**
 * @brief Represents a bulk data item from the Scryfall API.
 *
 * @see https://scryfall.com/docs/api/bulk-data
 */
struct BulkDataItem
{
  enum
  {
    SCRYFALL_BULK_DEFAULT_CARDS,
  } type;                   /**< Which bulk data item this struct represents */
  time_t updated_at;        /**< Last time this file was updated */
  size_t jsonl_size;        /**< Uncompressed size of the jsonl file */
  char *uri;                /**< URI for this file */
  char *jsonl_download_uri; /**< URI for the jsonl bulk data file */
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
 * @brief get_default_cards issues a request for the default card data from
 * scryfall.
 * @param[in] cfs Pointer to the CurlFatStruct to use for the request.
 *
 * @return int 1 if successful.
 */
int download_default_card_info (struct CurlFatStruct *cfs);

/**
 * @brief curl_deinit deinitializes the CurlFatStruct.
 * @param[in] cfs Pointer to the CurlFatStruct to deinitialize.
 */
void curl_deinit (struct CurlFatStruct *cfs);

#endif // HTTP_CLIENT_H_
