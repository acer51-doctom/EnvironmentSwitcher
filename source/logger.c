// source/logger.c
#include "logger.h"
#include "fsutils.h"
#include <coreinit/time.h>
#include <coreinit/debug.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

static FILE *log_file = NULL;

bool logger_init() {
    const char *log_base = "/fs/vol/external01/wiiu/environmentswitcher/logs";
    create_directory_if_missing("/fs/vol/external01/wiiu");
    create_directory_if_missing("/fs/vol/external01/wiiu/environmentswitcher");
    create_directory_if_missing(log_base);

    // Use ticks as unique timestamp fallback
    OSTime ticks = OSGetTime();

    char path[MAX_LOG_PATH];
    snprintf(path, sizeof(path), "%s/log_%llu.txt", log_base, ticks);

    log_file = fopen(path, "w");
    return log_file != NULL;
}

void logger_log(const char *tag, const char *message) {
    char line[MAX_LOG_LINE];
    snprintf(line, sizeof(line), "**[%s]** %s\n", tag, message);

    // Write to file
    if (log_file) {
        fputs(line, log_file);
        fflush(log_file);
    }

    // Print to screen (debug output)
    OSReport("%s", line);
}

void logger_close() {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}
