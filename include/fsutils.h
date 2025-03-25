// include/fsutils.h
#pragma once

#include <wut.h>
#include <stdbool.h>

#define MAX_ENVIRONMENTS 100
#define MAX_NAME_LENGTH 256

typedef struct {
    char name[MAX_NAME_LENGTH];
    bool is_aroma;
    bool is_valid;
} Environment;

/**
 * Scan for available environments in /fs/vol/external01/wiiu/environments/
 * Populates the provided array with up to max_envs entries.
 * Returns the number of environments found.
 */
int scan_environments(Environment *envs, int max_envs);

/**
 * Write content to a file safely (temporary -> replace).
 */
bool write_cfg_file_safe(const char *path, const char *content);

/**
 * Check if a file exists.
 */
bool file_exists(const char *path);

/**
 * Create directory (recursively) if it doesn’t exist.
 */
bool create_directory_if_missing(const char *path);
