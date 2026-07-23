/**
 * @file utils.h
 * @brief Utility for the momir vig project.
 */
#ifndef MOMIR_UTILS_H_
#define MOMIR_UTILS_H_

#include <stdbool.h>

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

#endif // MOMIR_UTILS_H_
