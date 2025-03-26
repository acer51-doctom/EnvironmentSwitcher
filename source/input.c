// source/input.c
#include "input.h"
#include <vpad/input.h>
#include <kpad/kpad.h>

static VPADStatus vpad_data;
static uint32_t prev_buttons = 0;
static uint32_t current_buttons = 0;

void input_init() {
    VPADInit();
    KPADInit();
}

void input_scan() {
    prev_buttons = current_buttons;
    current_buttons = 0;

    // --- GamePad ---
    VPADRead(0, &vpad_data, 0, NULL);
    current_buttons |= vpad_data.hold;

    // --- Wiimotes / Pro / Classic Controllers ---
    for (int i = 0; i < 4; i++) {
        KPADStatus kpad_data;
        int count = 0;
        KPADRead(i, &kpad_data, 1, &count);
        if (count > 0) {
            current_buttons |= kpad_data.hold;
        }
    }
}

bool input_pressed(uint32_t button) {
    return (current_buttons & button) && !(prev_buttons & button);
}
