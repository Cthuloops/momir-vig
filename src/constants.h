/**
 * @file constants.h
 * @brief Momir constant values.
 */
#ifndef MOMIR_CONSTANTS_H_
#define MOMIR_CONSTANTS_H_

// MISC  ----------------------------------------------------------------------
#define MOMIR_MAJOR_VERSION 0
#define MOMIR_MINOR_VERSION 0
#define MOMIR_PATCH_VERSION 1

#define MOMIR_DATA_DIR "./data/"
#define MOMIR_BULK_DATA_PATH MOMIR_DATA_DIR "bulk_data.json"
// MISC  ----------------------------------------------------------------------

// HTTP_CLIENT  ---------------------------------------------------------------
// Scryfall API
#define SCRYFALL_API_URL "https://api.scryfall.com"
#define SCRYFALL_BULK_URL SCRYFALL_API_URL "/bulk-data"
#define SCRYFALL_DEFAULT_CARDS_URL SCRYFALL_BULK_URL "/default-cards"

/**
 * @brief Required user agent for requests to Scryfall.
 *
 * @see https://scryfall.com/docs/api#required-headers
 */
#define SCRYFALL_HEADER_USER_AGENT "User-Agent: MomirVigApp/0.1"

/**
 * @brief Required accept for requests to Scryfall.
 *
 * @see https://scryfall.com/docs/api#required-headers
 */
#define SCRYFALL_ACCEPT "Accept: */*"

/**
 * @def SCRYFALL_BULK_DATA_UPDATE_TIME
 * @brief Amount of seconds in 7 days
 *
 * "If you only need gameplay information, downloading card data once per week
 * or right after set releases would most likely be sufficient."
 *
 * @see https://scryfall.com/docs/api/bulk-data
 */
#define SCRYFALL_BULK_DATA_UPDATE_TIME 604800L
// HTTP_CLIENT  ---------------------------------------------------------------

// MODULES  -------------------------------------------------------------------
#define MOMIR_MAIN "MOMIR_MAIN"
#define MOMIR_HTTP_CLIENT "MOMIR_HTTP_CLIENT"
#define MOMIR_PRINTER "MOMIR_PRINTER"
#define MOMIR_UTILS "MOMIR_UTILS"
#define MOMIR_BULK_DATA "MOMIR_BULK_DATA"
// MODULES  -------------------------------------------------------------------

#endif // MOMIR_CONSTANTS_H_
