// source/main.c
#include "fsutils.h"
#include "graphics.h"
#include "input.h"
#include "logger.h"
#include "environment.h"
#include <whb/proc.h>

#include <coreinit/screen.h>
#include <coreinit/exit.h>
#include <vpad/input.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_ENV 100

static char default_cfg_path[512];
static char autoboot_cfg_path[512];

static void wait_for_exit(bool confirmed) {
    if (!confirmed) {
        discard_cfg(default_cfg_path);
        discard_cfg(autoboot_cfg_path);
        logger_log("INFO", "Changes cancelled. Temp files discarded.");
        gfx_clear();
        gfx_draw_text(3, "Cancelled. No changes were made.");
        gfx_present();
        sleep(2);
        return;
    }

    logger_log("INFO", "Waiting 10 seconds...");
    logger_log("INFO", "Press A to reboot now or B to cancel.");

    for (int ticks = 1000; ticks > 0; --ticks) {
        input_scan();
        if (input_pressed(BTN_A)) {
            logger_log("INFO", "Rebooting immediately...");
            commit_cfg(default_cfg_path);
            commit_cfg(autoboot_cfg_path);
            WHBProcShutdown();
        } else if (input_pressed(BTN_B)) {
            discard_cfg(default_cfg_path);
            discard_cfg(autoboot_cfg_path);
            logger_log("INFO", "Cancelled by user. Temp files deleted.");
            gfx_clear();
            gfx_draw_text(3, "Cancelled. No changes were made.");
            gfx_present();
            sleep(2);
            return;
        }
        usleep(10000);
    }

    logger_log("INFO", "Rebooting after timeout.");
    commit_cfg(default_cfg_path);
    commit_cfg(autoboot_cfg_path);
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
            gfx_clear();
            gfx_draw_text(2, "Preparing environment switch...");
            gfx_present();

            const char *env_name = environments[selected].name;

            // Build paths
            snprintf(default_cfg_path, sizeof(default_cfg_path),
                "/fs/vol/external01/wiiu/environments/default.cfg");

            snprintf(autoboot_cfg_path, sizeof(autoboot_cfg_path),
                "/fs/vol/external01/wiiu/environments/%s/autoboot.cfg", env_name);

            // Write .tmp config files
            write_cfg_temp(default_cfg_path, env_name);
            logger_log("INFO", "Staged default.cfg");

            if (environments[selected].is_aroma) {
                write_cfg_temp(autoboot_cfg_path, "wiiu_menu");
                logger_log("INFO", "Staged autoboot.cfg for Aroma.");
                gfx_draw_text(4, "Autoboot set to Wii U Menu.");
                gfx_present();
                sleep(2);
                wait_for_exit(true);
                running = false;
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
                        write_cfg_temp(autoboot_cfg_path, "homebrew_launcher");
                        logger_log("INFO", "Staged autoboot.cfg for Tiramisu: HBL.");
                        decided = true;
                    } else if (input_pressed(BTN_B)) {
                        write_cfg_temp(autoboot_cfg_path, "wiiu_menu");
                        logger_log("INFO", "Staged autoboot.cfg for Tiramisu: Wii U Menu.");
                        decided = true;
                    }
                }

                gfx_clear();
                gfx_draw_text(3, "Environment ready to apply.");
                gfx_draw_text(4, "Press A to reboot, B to cancel.");
                gfx_present();

                wait_for_exit(true);
                running = false;
            }
        } else if (input_pressed(BTN_HOME)) {
            running = false;
        }

        usleep(10000);
    }

    logger_log("INFO", "Exiting EnvironmentSwitcher.");
    logger_close();
    return 0;
}
