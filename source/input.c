// source/input.c
#include "input.h"
#include <vpad/input.h>

static VPADStatus vpad_data;
static uint32_t prev_buttons = 0;

void input_init() {
    VPADInit();
}

void input_scan() {
    VPADRead(0, &vpad_data, 0, NULL);
}

bool input_pressed(uint32_t button) {
    uint32_t curr = vpad_data.hold;
    uint32_t down = curr & ~prev_buttons;
    prev_buttons = curr;
    return (down & button) != 0;
}
