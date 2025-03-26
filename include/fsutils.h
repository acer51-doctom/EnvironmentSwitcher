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
 */
int scan_environments(Environment *envs, int max_envs);

/**
 * Write to a .tmp file (safe version).
 * Example: write_cfg_temp("path/file.cfg", "content");
 */
bool write_cfg_temp(const char *path, const char *content);

/**
 * Commit a .tmp file to its final destination.
 * Example: commit_cfg("path/file.cfg");
 */
bool commit_cfg(const char *path);

/**
 * Delete the .tmp version of a file.
 * Example: discard_cfg("path/file.cfg");
 */
bool discard_cfg(const char *path);

/**
 * Check if a file exists.
 */
bool file_exists(const char *path);

/**
 * Create directory (recursively) if it doesn’t exist.
 */
bool create_directory_if_missing(const char *path);
