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
 * Scan for environments in /wiiu/environments/
 * Fills the envs array up to max_envs.
 */
int scan_environments(Environment *envs, int max_envs);

/**
 * Write to a .tmp file (safe operation).
 * Example: write_cfg_temp("/path/to/file.cfg", "envName");
 */
bool write_cfg_temp(const char *path, const char *content);

/**
 * Rename the .tmp file to its final path (commit the change).
 */
bool commit_cfg(const char *path);

/**
 * Delete the .tmp file if the user cancels (revert).
 */
bool discard_cfg(const char *path);

/**
 * Check if a file exists.
 */
bool file_exists(const char *path);

/**
 * Create a directory if it doesn't already exist.
 */
bool create_directory_if_missing(const char *path);
