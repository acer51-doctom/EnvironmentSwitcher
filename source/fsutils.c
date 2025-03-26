// source/fsutils.c
#include "fsutils.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define SD_ROOT "/fs/vol/external01"

bool file_exists(const char *path) {
    FILE *f = fopen(path, "r");
    if (f) {
        fclose(f);
        return true;
    }
    return false;
}

bool create_directory_if_missing(const char *path) {
    struct stat st;
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
        return true;
    }
    return mkdir(path, 0777) == 0;
}

bool write_cfg_temp(const char *path, const char *content) {
    char tmp_path[512];
    snprintf(tmp_path, sizeof(tmp_path), "%s.tmp", path);

    FILE *f = fopen(tmp_path, "w");
    if (!f) return false;

    fwrite(content, 1, strlen(content), f);
    fclose(f);
    return true;
}

bool commit_cfg(const char *path) {
    char tmp_path[512];
    snprintf(tmp_path, sizeof(tmp_path), "%s.tmp", path);

    if (!file_exists(tmp_path)) return false;

    remove(path); // remove existing file, if any
    return rename(tmp_path, path) == 0;
}

bool discard_cfg(const char *path) {
    char tmp_path[512];
    snprintf(tmp_path, sizeof(tmp_path), "%s.tmp", path);

    if (file_exists(tmp_path)) {
        return remove(tmp_path) == 0;
    }
    return true;
}

int scan_environments(Environment *envs, int max_envs) {
    char base_path[512];
    snprintf(base_path, sizeof(base_path), "%s/wiiu/environments", SD_ROOT);

    DIR *dir = opendir(base_path);
    if (!dir) return 0;

    int count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) && count < max_envs) {
        if (entry->d_type != DT_DIR)
            continue;

        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0 ||
            strcasecmp(entry->d_name, "installer") == 0)
            continue;

        Environment *env = &envs[count];
        memset(env, 0, sizeof(Environment));
        strncpy(env->name, entry->d_name, MAX_NAME_LENGTH - 1);

        // Detect Aroma via /plugins folder with .wps and /config
        char plugin_path[512];
        snprintf(plugin_path, sizeof(plugin_path), "%s/%s/plugins", base_path, env->name);

        DIR *plugin_dir = opendir(plugin_path);
        if (plugin_dir) {
            struct dirent *plugin_entry;
            while ((plugin_entry = readdir(plugin_dir))) {
                if (strstr(plugin_entry->d_name, ".wps")) {
                    char config_path[512];
                    snprintf(config_path, sizeof(config_path), "%s/config", plugin_path);
                    struct stat st;
                    if (stat(config_path, &st) == 0 && S_ISDIR(st.st_mode)) {
                        env->is_aroma = true;
                        break;
                    }
                }
            }
            closedir(plugin_dir);
        }

        env->is_valid = true;
        count++;
    }

    closedir(dir);
    return count;
}
