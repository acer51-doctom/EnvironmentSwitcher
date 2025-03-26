// include/input.h
#pragma once

#include <wut.h>
#include <stdbool.h>

/**
 * Initializes the VPAD input system.
 */
void input_init();

/**
 * Scans for new input from the GamePad.
 */
void input_scan();

/**
 * Returns true if the given button was just pressed.
 */
bool input_pressed(uint32_t button);

/**
 * Button constants (VPAD_BUTTON_*)
 */
#define BTN_A     VPAD_BUTTON_A
#define BTN_B     VPAD_BUTTON_B
#define BTN_UP    VPAD_BUTTON_UP
#define BTN_DOWN  VPAD_BUTTON_DOWN
#define BTN_HOME  VPAD_BUTTON_HOME
#define BTN_PLUS  VPAD_BUTTON_PLUS
