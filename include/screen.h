#pragma once

#include <wut.h>
#include <coreinit/screen.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static float __fontSize = 20.0f;

/**
 * Initializes the OSScreen subsystem and sets up buffers.
 */
static inline void WHBScreenInit(void) {
    OSScreenInit();
    OSScreenSetBufferEx(SCREEN_TV, (void *)0xF4000000);
    OSScreenEnableEx(SCREEN_TV, true);
}

/**
 * Shuts down the screen (not required for OSScreen).
 */
static inline void WHBScreenShutdown(void) {
    // No-op for OSScreen
}

/**
 * Clears the screen to black.
 */
static inline void WHBScreenClear(void) {
    OSScreenClearBufferEx(SCREEN_TV, 0);
}

/**
 * Flips the buffer to display content.
 */
static inline void WHBScreenFlip(void) {
    OSScreenFlipBuffersEx(SCREEN_TV);
}

/**
 * Sets the font size (not used by OSScreen, but we'll track it).
 */
static inline void WHBScreenSetFontSize(float size) {
    __fontSize = size;
}

/**
 * Draws a string to the screen at the given (x, y) position in pixels.
 */
static inline void WHBScreenPrint(float x, float y, const char *text) {
    if (!text) return;
    OSScreenPutFontEx(SCREEN_TV, (int)x, (int)y, text);
}

#ifdef __cplusplus
}
#endif
