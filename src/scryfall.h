#ifndef SCRYFALL_H_
#define SCRYFALL_H_

#include <stdlib.h>

#include <curl/curl.h>

struct CurlFatStruct {
    CURL *curl;
    char *memory;
    size_t size;
};


/**
 * @brief curl_init initializes curl.
 * @param[in] cfs Pointer to CurlFatStruct instance to manage.
 *
 * Initializes the CurlFatStruct struct to be used for later transfers.
 *
 * @return int 1 if successful.
 */
int curl_init(struct CurlFatStruct *cfs);


/**
 * @brief scryfall_bulk_data issues a request for the bulk_data information.
 * @param[in] cfs Pointer to the CurlFatStruct to use for the request.
 *
 * Downloads the json for the bulk_data api information from scryfall. This
 * gets stored in the CurlFatStruct.
 *
 * @returns int 1 if successul.
 */
int scryfall_bulk_data(struct CurlFatStruct *cfs);


/**
 * @brief curl_deinit deinitializes the CurlFatStruct.
 * @param[in] cfs Pointer to the CurlFatStruct to deinitialize.
 *
 * Does nothing if the pointer is null or curl is unintialized.
 */
void curl_deinit(struct CurlFatStruct *cfs);


#endif // SCRYFALL_H_
