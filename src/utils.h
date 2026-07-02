/**
 * @file utils.h
 * @brief Utility for the momir vig project.
 */
#ifndef MOMIR_UTILS_H_
#define MOMIR_UTILS_H_

#include <stdbool.h>

#include "http_client.h"

/**
 * @brief file_exists checks if a file at the given path exists
 *
 * @param[in] file_path Path to the file.
 * @param[in] is_dir If true, look for directory, else look for regular file.
 *
 * @return True if file exists, false otherwise.
 */
bool file_exists (const char *file_path, bool is_dir);

/**
 * @brief create_dir creates a directory at the given path.
 *
 * @param[in] dir_path Path to the directory.
 *
 * @return True if the directory exists/created, false otherwise.
 */
bool create_dir (const char *dir_path);

/**
 * @brief bulk_file_needs_update checks if the bulk file needs to be updated.
 *
 * @param[in] file_path Path to the bulk file.
 *
 * @return True if SCRYFALL_UPDATE_TIME has passed since the file was last,
 * updated. False otherwise, or if there was an error.
 */
bool bulk_file_needs_update (const char *file_path);

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
bool bulk_data_item_up_to_date (const struct BulkDataItem *bdi);

#endif // MOMIR_UTILS_H_
