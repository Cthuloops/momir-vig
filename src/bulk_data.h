#ifndef MOMIR_BULK_DATA_H_
#define MOMIR_BULK_DATA_H_

#include <stdbool.h>
#include <stddef.h>
#include <time.h>

#include "constants.h"

const size_t BulkDataItem_str_size = 1;
const char *BulkDataItem_str[] = { "default_cards" };

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
    UNKNOWN
  } type;                   /**< Which bulk data item this struct represents */
  char *type_name;          /**< Type field from bulk data object */
  time_t updated_at;        /**< Last time this file was updated */
  size_t jsonl_size;        /**< Uncompressed size of the jsonl file */
  char *uri;                /**< URI for this file */
  char *jsonl_download_uri; /**< URI for the jsonl bulk data file */
};

/**
 * @brief bulk_data_item_up_to_date checks if the data item is considered up to
 * date.
 *
 * Checks to see if a data item has been updated within
 * #SCRYFALL_BULK_DATA_UPDATE_TIME
 *
 * @param[in] bdi Pointer to a BulkDataItem struct.
 *
 * @return true if the data is considered up to date, false otherwise.
 *
 * @see struct BulkDataItem
 */
[[maybe_unused]] static inline bool
bulk_data_item_up_to_date (const struct BulkDataItem *bdi)
{
  return difftime (time (NULL), bdi->updated_at)
         < SCRYFALL_BULK_DATA_UPDATE_TIME;
}

/**
 * @brief bulk_data_item_from_string creates a BulkDataItem from a bulk data
 * item json string.
 *
 * Expects a json object with the fields found in a #BulkDataItem struct.
 *
 * @see bulk_item_free
 *
 * @return #bulkDataItem or NULL.
 */
struct BulkDataItem *bulk_item_from_string (const char *json_string);

/**
 * @brief bulk_item_from_file is a wrapper of bulk_item_from_string.
 *
 * @see bulk_item_from_string
 *
 * @return #BulkDataItem or NULL
 */
struct BulkDataItem *bulk_item_from_file (const char *file_path);

void bulk_item_free (struct BulkDataItem *bdi);

#endif // MOMIR_BULK_DATA_H_
