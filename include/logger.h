// include/logger.h
#pragma once

#include <wut.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_LOG_LINE 512
#define MAX_LOG_PATH 512

/**
 * Initialize logging system.
 * Opens the log file and creates necessary directories.
 */
bool logger_init();

/**
 * Log a line to screen and file.
 * Format: "**[TAG]** Message"
 */
void logger_log(const char *tag, const char *message);

/**
 * Closes the current log file.
 */
void logger_close();
