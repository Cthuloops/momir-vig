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
 * @brief curl_init initializes curl.
 * @param[in] cfs Pointer to CurlFatStruct instance to manage.
 *
 * Initializes the CurlFatStruct struct to be used for later transfers.
 *
 * @return int 1 if successful. Otherwise, integer error from libcurl.
 */
int curl_init (struct CurlFatStruct *cfs);

/**
 * @brief get_default_cards issues a request for the default card data from
 * scryfall.
 * @param[in] cfs Pointer to the CurlFatStruct to use for the request.
 *
 * @return int 1 if successful. -1 on failure before request. Otherwise,
 *  integer error from libcurl.
 */
int download_default_card_info (struct CurlFatStruct *cfs);

/**
 * @brief curl_deinit deinitializes the CurlFatStruct.
 * @param[in] cfs Pointer to the CurlFatStruct to deinitialize.
 */
void curl_deinit (struct CurlFatStruct *cfs);

#endif // HTTP_CLIENT_H_
