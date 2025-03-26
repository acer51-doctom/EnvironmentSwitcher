// source/environment.c
#include "environment.h"
#include <string.h>
#include <strings.h>

bool is_base_environment(const char *name) {
    if (!name) return false;
    return strcasecmp(name, "aroma") == 0 || strcasecmp(name, "tiramisu") == 0;
}

const char* get_environment_descriptor(const Environment *env) {
    if (!env || !env->is_valid || is_base_environment(env->name)) {
        return ""; // No tag if it's "aroma" or "tiramisu"
    }

    if (env->is_aroma) {
        return "(Aroma based)";
    } else {
        return "(Tiramisu/Legacy detected.)";
    }
}

bool is_tiramisu_based(const Environment *env) {
    if (!env || !env->is_valid) return false;
    return !env->is_aroma;
}
