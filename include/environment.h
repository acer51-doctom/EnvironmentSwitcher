// include/environment.h
#pragma once

#include "fsutils.h"
#include <stdbool.h>

/**
 * Determines if the environment folder is literally "aroma" or "tiramisu".
 * Used to suppress "(Aroma based)" or "(Tiramisu/Legacy detected.)".
 */
bool is_base_environment(const char *name);

/**
 * Returns a human-readable label based on environment type.
 * - "(Aroma based)"
 * - "(Tiramisu/Legacy detected.)"
 * - "" if it's named aroma/tiramisu
 */
const char* get_environment_descriptor(const Environment *env);

/**
 * Returns true if it's Tiramisu or Legacy (not Aroma).
 * Used for boot prompt.
 */
bool is_tiramisu_based(const Environment *env);
