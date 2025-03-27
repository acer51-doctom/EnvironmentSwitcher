#pragma once

#include <wut.h>
#include <coreinit/screen.h>
#include <coreinit/memdefaultheap.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static float __fontSize = 20.0f;
static void* screenBuffer = NULL;

/**
 * Initializes the screen system and allocates buffer from MEM1.
 */
static inline void WHBScreenInit(void) {
    OSScreenInit();

    uint32_t bufferSize = OSScreenGetBufferSizeEx(SCREEN_TV);
    screenBuffer = MEM1AllocFromDefaultHeapEx(bufferSize, 0x100);

    if (screenBuffer) {
        OSScreenSetBufferEx(SCREEN_TV, screenBuffer);
        OSScreenEnableEx(SCREEN_TV, true);
    }
}

/**
 * Clears the screen to black.
 */
static inline void WHBScreenClear(void) {
    OSScreenClearBufferEx(SCREEN_TV, 0);
}

/**
 * Flips the screen buffers (draw to screen).
 */
static inline void WHBScreenFlip(void) {
    OSScreenFlipBuffersEx(SCREEN_TV);
}

/**
 * Set the font size for drawing (used by WHB-style API only).
 */
static inline void WHBScreenSetFontSize(float size) {
    __fontSize = size; // Stored, not used by OSScreen directly
}

/**
 * Draws a string on screen at pixel (x, y).
 */
static inline void WHBScreenPrint(float x, float y, const char *text) {
    if (!text) return;
    OSScreenPutFontEx(SCREEN_TV, (int)x, (int)y, text);
}

/**
 * Shuts down and frees the buffer.
 */
static inline void WHBScreenShutdown(void) {
    if (screenBuffer) {
        MEM1FreeToDefaultHeap(screenBuffer);
        screenBuffer = NULL;
    }
}

#ifdef __cplusplus
}
#endif
