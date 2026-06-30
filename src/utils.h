#ifndef MOMIR_UTILS_H_
#define MOMIR_UTILS_H_

#include <stdbool.h>

/**
 * @brief file_exists checks if a file at the given path exists, optionally
 * creates the file if it's missing.
 * @param[in] file_path Path to the file.
 * @param[in] is_dir If true, look for directory, else look for regular file.
 *
 * @return True if file exists, false otherwise.
 */
bool file_exists (const char *file_path, bool is_dir);

/**
 * @brief create_dir creates a directory at the given path.
 * @param[in] dir_path Path to the directory.
 *
 * @return True if the directory exists/created, false otherwise.
 */
bool create_dir (const char *dir_path);

/**
 * @brief bulk_file_needs_update checks if the bulk file needs to be updated.
 * @param[in] file_path Path to the bulk file.
 *
 * @return True if SCRYFALL_UPDATE_TIME has passed since the file was last,
 * updated. False otherwise, or if there was an error.
 */
bool bulk_file_needs_update (const char *file_path);

#endif // MOMIR_UTILS_H_
