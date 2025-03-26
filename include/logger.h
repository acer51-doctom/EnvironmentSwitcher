// include/logger.h
#pragma once

#include <wut.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_LOG_LINE 512
#define MAX_LOG_PATH 512

/**
 * Initialize the logging system.
 * Creates log directory + opens a new log file with timestamp.
 */
bool logger_init();

/**
 * Log a line to both screen and file.
 * Auto-formats as: **[TAG]** message
 */
void logger_log(const char *tag, const char *message);

/**
 * Close the log file cleanly.
 */
void logger_close();
