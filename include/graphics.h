// include/graphics.h
#pragma once

#include "fsutils.h"
#include <wut.h>

/**
 * Initializes the graphics system.
 */
void gfx_init();

/**
 * Clears the screen to black.
 */
void gfx_clear();

/**
 * Renders a line of text at the given row (Y index), monospaced.
 * Automatically calculates Y position based on row number.
 */
void gfx_draw_text(int row, const char *text);

/**
 * Draws the welcome screen message.
 */
void gfx_draw_welcome();

/**
 * Draws the environment list with the selector.
 */
void gfx_draw_env_list(Environment *envs, int env_count, int selected_index);

/**
 * Flushes and displays everything drawn.
 */
void gfx_present();
