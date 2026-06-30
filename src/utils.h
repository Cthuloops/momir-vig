#ifndef MOMIR_UTILS_H_
#define MOMIR_UTILS_H_

#include <stdbool.h>

/**
 * @brief file_exists checks if a file at the given path exists, optionally
 * creates the file if it's missing.
 * @param[in] file_path Path to the file.
 * @param[in] is_dir If true, look for directory, else look for regular file.
 * @param[in] create If true, create the directory/file if it isn't found
 * (directory permissions = 0755, file permissions = 0644).
 *
 * @return True if file exists or is created, false otherwise.
 */
bool file_exists (const char *file_path, bool is_dir, bool create);

/**
 * @brief bulk_file_needs_update checks if the bulk file needs to be updated.
 * @param[in] file_path Path to the bulk file.
 *
 * @return True if SCRYFALL_UPDATE_TIME has passed since the file was last,
 * updated. False otherwise, or if there was an error.
 */
bool bulk_file_needs_update (const char *file_path);

#endif // MOMIR_UTILS_H_
