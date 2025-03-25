// source/logger.c
#include "logger.h"
#include "fsutils.h"
#include <coreinit/time.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

static FILE *log_file = NULL;

bool logger_init() {
    const char *log_base = "/fs/vol/external01/wiiu/environmentswitcher/logs";
    create_directory_if_missing("/fs/vol/external01/wiiu");
    create_directory_if_missing("/fs/vol/external01/wiiu/environmentswitcher");
    create_directory_if_missing(log_base);

    // Generate timestamp filename
    OSTime time = OSGetTime();
    OSTimeToCalendarTime(time, 0);
    OSCalendarTime calendar;
    OSTicksToCalendarTime(OSGetTime(), &calendar);

    char path[MAX_LOG_PATH];
    snprintf(path, sizeof(path),
        "%s/%02d-%02d-%04d-%02d-%02d-%02d.txt",
        log_base,
        calendar.tm_mday,
        calendar.tm_mon + 1,
        calendar.tm_year + 1900,
        calendar.tm_hour,
        calendar.tm_min,
        calendar.tm_sec
    );

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

    // Optional: echo to screen — to be wired up in graphics module
    OSReport("%s", line);
}

void logger_close() {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}
