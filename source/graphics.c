// source/graphics.c
#include "graphics.h"
#include "environment.h"
#include <coreinit/screen.h>
#include <stdio.h>
#include <string.h>

#define MAX_ROW_LENGTH 100
#define BASE_Y 40
#define LINE_HEIGHT 24
#define SCREEN_ID SCREEN_TV

void gfx_init() {
    OSScreenInit();
    OSScreenSetBufferEx(SCREEN_TV, (void *)0xF4000000);
    OSScreenEnableEx(SCREEN_TV, true);
}

void gfx_clear() {
    OSScreenClearBufferEx(SCREEN_ID, 0);  // 0 = black
}

void gfx_draw_text(int row, const char *text) {
    if (!text) return;

    int y = BASE_Y + row * LINE_HEIGHT;
    OSScreenPutFontEx(SCREEN_ID, 0, y, text);
}

void gfx_draw_welcome() {
    gfx_clear();
    gfx_draw_text(1, "Welcome to EnvironmentSwitcher!");
    gfx_draw_text(2, "Select your environment with the D-Pad");
    gfx_draw_text(3, "and press A to confirm!");
    gfx_present();
}

void gfx_draw_env_list(Environment *envs, int env_count, int selected_index) {
    gfx_clear();
    gfx_draw_text(0, "Available Environments:");

    for (int i = 0; i < env_count; ++i) {
        char line[MAX_ROW_LENGTH];
        const char *descriptor = get_environment_descriptor(&envs[i]);

        snprintf(line, sizeof(line), "%s %s %s",
                 (i == selected_index ? "-->" : "   "),
                 envs[i].name,
                 descriptor);

        gfx_draw_text(i + 2, line); // +2 to leave space at top
    }

    gfx_present();
}

void gfx_present() {
    OSScreenFlipBuffersEx(SCREEN_ID);
}
