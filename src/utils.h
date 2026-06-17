#ifndef MOMIR_UTILS_H_
#define MOMIR_UTILS_H_

#include <stdbool.h>

#define SCRYFALL_BULK_DATA_UPDATE_TIME 24

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

#endif // MOMIR_UTILS_H_
