// include/graphics.h
#pragma once

#include "fsutils.h"
#include <wut.h>

/**
 * Initialize WHB screen rendering.
 * Must be called before any draw functions.
 */
void gfx_init();

/**
 * Clear the screen to black.
 */
void gfx_clear();

/**
 * Draw a single line of text on screen at the given row (monospaced layout).
 */
void gfx_draw_text(int row, const char *text);

/**
 * Show the initial welcome screen message.
 */
void gfx_draw_welcome();

/**
 * Render a list of environments with a selector arrow (-->) at the current index.
 */
void gfx_draw_env_list(Environment *envs, int env_count, int selected_index);

/**
 * Present everything drawn to the display (flip buffer).
 */
void gfx_present();
