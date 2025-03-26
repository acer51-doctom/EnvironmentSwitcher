// source/main.c
#include "fsutils.h"
#include "graphics.h"
#include "input.h"
#include "logger.h"
#include "environment.h"
#include "screen.h"  // Your custom WHB-style screen wrapper
#include <whb/proc.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_ENV 100

static char default_cfg_path[512];
static char autoboot_cfg_path[512];

static void wait_for_exit(bool confirmed) {
    if (!confirmed) {
        discard_cfg(default_cfg_path);
        discard_cfg(autoboot_cfg_path);
        logger_log("INFO", "User cancelled. Temporary files discarded.");
        gfx_clear();
        gfx_draw_text(3, "Cancelled. No changes were made.");
        gfx_present();
        sleep(2);
        return;
    }

    logger_log("INFO", "Waiting 10 seconds...");
    logger_log("INFO", "Press A to reboot now or B to cancel.");

    for (int i = 0; i < 1000; ++i) {
        input_scan();
        if (input_pressed(BTN_A)) {
            logger_log("INFO", "Rebooting immediately...");
            commit_cfg(default_cfg_path);
            commit_cfg(autoboot_cfg_path);
            WHBProcShutdown();
        } else if (input_pressed(BTN_B)) {
            discard_cfg(default_cfg_path);
            discard_cfg(autoboot_cfg_path);
            logger_log("INFO", "User cancelled during reboot timer.");
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

    create_directory_if_missing("/fs/vol/external01/wiiu/environments");
    create_directory_if_missing("/fs/vol/external01/wiiu/environmentswitcher");
    create_directory_if_missing("/fs/vol/external01/wiiu/environmentswitcher/logs");

    gfx_draw_welcome();
    sleep(2);

    Environment environments[MAX_ENV];
    int env_count = scan_environments(environments, MAX_ENV);

    if (env_count == 0) {
        gfx_clear();
        gfx_draw_text(2, "No environments found.");
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

            snprintf(default_cfg_path, sizeof(default_cfg_path),
                "/fs/vol/external01/wiiu/environments/default.cfg");

            snprintf(autoboot_cfg_path, sizeof(autoboot_cfg_path),
                "/fs/vol/external01/wiiu/environments/%s/autoboot.cfg", env_name);

            // Make sure environment dir exists
            char env_dir[512];
            snprintf(env_dir, sizeof(env_dir),
                "/fs/vol/external01/wiiu/environments/%s", env_name);
            create_directory_if_missing(env_dir);

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
                gfx_clear();
                gfx_draw_text(2, "Where do you want to boot?");
                gfx_draw_text(3, "- Press A for Homebrew Launcher");
                gfx_draw_text(4, "- Press B for Wii U Menu");
                gfx_draw_text(5, "(Hold + when selecting to show this menu)");
                gfx_present();

                bool decided = false;
                while (!decided) {
                    input_scan();
                    if (input_pressed(BTN_A)) {
                        write_cfg_temp(autoboot_cfg_path, "homebrew_launcher");
                        logger_log("INFO", "Staged autoboot.cfg to homebrew_launcher.");
                        decided = true;
                    } else if (input_pressed(BTN_B)) {
                        write_cfg_temp(autoboot_cfg_path, "wiiu_menu");
                        logger_log("INFO", "Staged autoboot.cfg to wiiu_menu.");
                        decided = true;
                    }
                }

                gfx_clear();
                gfx_draw_text(3, "Ready to switch environment.");
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
