#ifndef MOMIR_GUARD_H_
#define MOMIR_GUARD_H_

#include "logging.h"

#define PARAMETER_NULL_GUARD(ptr, module, target_label)                       \
  do                                                                          \
    {                                                                         \
      if (!(ptr))                                                             \
        {                                                                     \
          LOG_ERROR ((module), "Parameter %s was NULL in %s", #ptr,           \
                     __func__);                                               \
          goto target_label;                                                  \
        }                                                                     \
    }                                                                         \
  while (0)

#define PARAMETER_EMPTY_STRING_GUARD(ptr, module, target_label)               \
  do                                                                          \
    {                                                                         \
      if (strcmp (ptr, "") == 0)                                              \
        {                                                                     \
          LOG_ERROR ((module), "Passed empty string for %s in %s", #ptr,      \
                     __func__);                                               \
          goto target_label;                                                  \
        }                                                                     \
    }                                                                         \
  while (0)

#define PARAMETER_STRING_GUARD(ptr, module, target_label)                     \
  do                                                                          \
    {                                                                         \
      PARAMETER_NULL_GUARD (ptr, module, target_label);                       \
      PARAMETER_EMPTY_STRING_GUARD (ptr, module, target_label);               \
    }                                                                         \
  while (0)

#endif // MOMIR_GUARD_H_
