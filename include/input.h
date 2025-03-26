// include/input.h
#pragma once

#include <wut.h>
#include <stdbool.h>

/**
 * Initialize VPAD and KPAD input systems.
 */
void input_init();

/**
 * Poll input from GamePad and supported controllers (Wiimote/Pro/Classic).
 */
void input_scan();

/**
 * Return true if the given button was just pressed on any controller.
 */
bool input_pressed(uint32_t button);

/**
 * Unified button constants (VPAD & KPAD-compatible).
 */
#define BTN_A     0x00000400
#define BTN_B     0x00000200
#define BTN_UP    0x00000008
#define BTN_DOWN  0x00000004
#define BTN_HOME  0x00000800
#define BTN_PLUS  0x00001000
