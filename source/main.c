// source/main.c
#include "fsutils.h"
#include "graphics.h"
#include "input.h"
#include "logger.h"
#include "environment.h"

#include <coreinit/screen.h>
#include <coreinit/exit.h>
#include <vpad/input.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_ENV 100

static void wait_for_exit() {
    logger_log("INFO", "Waiting 10 seconds...");
    logger_log("INFO", "Press A to reboot now or B to cancel.");
    
    int ticks = 1000; // Approx 10 sec if ~10ms per iteration

    while (ticks-- > 0) {
        input_scan();
        if (input_pressed(BTN_A)) {
            logger_log("INFO", "Rebooting immediately...");
            WHBProcShutdown();
        } else if (input_pressed(BTN_B)) {
            logger_log("INFO", "Cancelled by user.");
            return;
        }
        usleep(10000);
    }

    logger_log("INFO", "Auto-rebooting after delay...");
    WHBProcShutdown();
}

int main(int argc, char **argv) {
    gfx_init();
    input_init();
    logger_init();

    gfx_draw_welcome();
    sleep(2);

    Environment environments[MAX_ENV];
    int env_count = scan_environments(environments, MAX_ENV);

    if (env_count == 0) {
        gfx_clear();
        gfx_draw_text(2, "No valid environments found.");
        logger_log("ERROR", "No valid environments found.");
        gfx_present();
        sleep(3);
        logger_close();
        return 0;
    }

    int selected = 0;
    bool running = true;

    while (running) {
        input_scan();
        gfx_draw_env_list(environments, env_count, selected);

        if (input_pressed(BTN_DOWN)) {
            selected = (selected + 1) % env_count;
        } else if (input_pressed(BTN_UP)) {
            selected = (selected - 1 + env_count) % env_count;
        } else if (input_pressed(BTN_A)) {
            // Start verbose log
            gfx_clear();
            char msg[128];
            snprintf(msg, sizeof(msg), "Selected environment: %s", environments[selected].name);
            logger_log("INFO", msg);

            // Write default.cfg
            const char *env_name = environments[selected].name;
            char def_cfg[512];
            snprintf(def_cfg, sizeof(def_cfg), "/fs/vol/external01/wiiu/environments/default.cfg");

            if (write_cfg_file_safe(def_cfg, env_name)) {
                logger_log("INFO", "Modified default.cfg file...");
            } else {
                logger_log("ERROR", "Failed to write default.cfg");
            }

            // Write autoboot.cfg
            char auto_path[512];
            snprintf(auto_path, sizeof(auto_path),
                "/fs/vol/external01/wiiu/environments/%s/autoboot.cfg", env_name);

            if (environments[selected].is_aroma) {
                write_cfg_file_safe(auto_path, "wiiu_menu");
                logger_log("INFO", "Set autoboot to wiiu_menu for Aroma.");
            } else {
                // Tiramisu prompt
                gfx_clear();
                gfx_draw_text(2, "Where do you want to boot?");
                gfx_draw_text(3, "- Press A for Homebrew Launcher");
                gfx_draw_text(4, "- Press B for Wii U Menu");
                gfx_draw_text(5, "(Hold + when selected to see this menu again)");
                gfx_present();

                bool decided = false;
                while (!decided) {
                    input_scan();
                    if (input_pressed(BTN_A)) {
                        write_cfg_file_safe(auto_path, "homebrew_launcher");
                        logger_log("INFO", "Set autoboot to homebrew_launcher for Tiramisu.");
                        decided = true;
                    } else if (input_pressed(BTN_B)) {
                        write_cfg_file_safe(auto_path, "wiiu_menu");
                        logger_log("INFO", "Set autoboot to wiiu_menu for Tiramisu.");
                        decided = true;
                    }
                }
            }

            logger_log("SUCCESS", "Environment switch complete.");
            gfx_clear();
            gfx_draw_text(3, "Environment switched successfully.");
            gfx_draw_text(4, "Press A to reboot, B to cancel.");
            gfx_present();

            wait_for_exit();
            running = false;
        } else if (input_pressed(BTN_HOME)) {
            running = false;
        }

        usleep(10000);
    }

    logger_log("INFO", "Exiting EnvironmentSwitcher.");
    logger_close();
    return 0;
}
