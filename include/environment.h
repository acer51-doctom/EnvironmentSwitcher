// include/environment.h
#pragma once

#include "fsutils.h"
#include <stdbool.h>

/**
 * Determines if the environment name is literally "aroma" or "tiramisu".
 * Used to suppress the "(Aroma based)" or "(Tiramisu/Legacy detected.)" tag.
 */
bool is_base_environment(const char *name);

/**
 * Returns a human-readable descriptor based on detection flags.
 * Example: "(Aroma based)", "(Tiramisu/Legacy detected.)", or "" if suppressed.
 */
const char* get_environment_descriptor(const Environment *env);

/**
 * Determines if the selected environment is Tiramisu-based (but not Aroma).
 * Used for autoboot prompting.
 */
bool is_tiramisu_based(const Environment *env);
