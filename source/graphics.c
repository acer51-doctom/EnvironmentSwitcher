// source/graphics.c
#include "graphics.h"
#include "environment.h"
#include <whb/screen.h>
#include <stdio.h>
#include <string.h>

#define MAX_ROW_LENGTH 100
#define BASE_Y 40.0f
#define LINE_HEIGHT 24.0f
#define TEXT_X 30.0f

void gfx_init() {
    WHBScreenInit();
    WHBScreenSetFontSize(20.0f);
}

void gfx_clear() {
    WHBScreenClear();
}

void gfx_draw_text(int row, const char *text) {
    if (!text) return;
    float y = BASE_Y + row * LINE_HEIGHT;
    WHBScreenPrint(TEXT_X, y, text);
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

        gfx_draw_text(i + 2, line);
    }

    gfx_present();
}

void gfx_present() {
    WHBScreenFlip();
}
