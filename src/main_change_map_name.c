/* main_change_map_name @ 0x83689F08 — drive the main-menu -> in-game map transition. While the main-menu
 * scenario is still loaded it runs the fade-out: if a load timer is pending it sets the fade value from
 * the remaining time; otherwise, once the menu music is active, it kicks off a 1-second screen fade and
 * inhibits widget processing. It waits (returns early) until the load timer elapses. Then, unless we are
 * instead fading to the dashboard, it tears down the main menu (stop music, disable menu, re-enable input)
 * and — for a non-networked game in progress — loads the chosen solo map: dispose the old map, precache
 * and load the new one, and record it as each player's last level played.
 *
 * The database gives no prototype; the function takes no parameters (the decompiler's a1/a2/a3 are
 * register artifacts). */

#include <stdint.h>
#include <string.h>
#include "headers/main_globals.h"
#include "headers/game_globals.h"   /* game_options */
#include "headers/blam_data_globals.h"


extern void ui_loading_init_state(void);
extern void ui_loading_set_state(int state);
extern uint8_t ui_main_menu_music_active(void);
extern void main_screen_shell_begin_fade(unsigned int fade_duration_milliseconds);
extern void ui_widgets_inhibit_processing(uint8_t inhibit);
extern void ui_widgets_set_fade_value(float value);
extern void ui_stop_main_menu_music(void);
extern void main_menu_active(uint8_t active);
extern void input_abstraction_set_enabled_mode_events(uint8_t enabled);
extern uint8_t game_in_progress(void);
extern void game_options_new(game_options *options);
extern void game_dispose_from_old_map(void);
extern void game_precache_new_map(const char *map_name, uint8_t blocking);
extern void game_unload(void);
extern void main_new_map(game_options *options);
extern void player_profile_save_last_level_played(int16_t local_player_index);

void main_change_map_name(void)
{
    game_options options;

    if ( !main_globals.connection )
    {
        ui_loading_init_state();
        ui_loading_set_state(1);
    }

    if ( main_globals.main_menu_scenario_loaded == 1 )
    {
        if ( main_globals.map_change_load_timer )
        {
            unsigned int remaining = main_globals.map_change_load_timer - main_globals.last_time_msec;
            ui_widgets_set_fade_value(-((float)remaining * 0.001f - 1.0f));
        }
        else if ( ui_main_menu_music_active() == 1 )
        {
            main_globals.map_change_load_timer = 0;
            main_screen_shell_begin_fade(1000);
            ui_widgets_inhibit_processing(1);
            ui_widgets_set_fade_value(0.0f);
        }

        if ( main_globals.last_time_msec < main_globals.map_change_load_timer )
            return;
    }
    else
    {
        main_globals.map_change_load_timer = 0;
    }

    if ( main_globals.fade_to_dashboard )
    {
        main_globals.want_to_be_at_main_menu = 0;
        main_globals.exit_to_dashboard = 1;
        main_globals.map_change_load_timer = 0;
        return;
    }

    ui_widgets_set_fade_value(-1.0f);
    ui_stop_main_menu_music();
    main_menu_active(0);
    main_globals.main_menu_scenario_loaded = 0;
    input_abstraction_set_enabled_mode_events(0);
    ui_widgets_inhibit_processing(0);

    if ( game_in_progress() && !main_globals.connection )
    {
        int16_t i;

        game_options_new(&options);
        strncpy(options.map_name, main_globals.soloplayer_map_name, 0xFF);
        options.map_name[255] = 0;
        options.difficulty = global_difficulty_level;
        /* DEVIATION: decompiler passed strncpy's return (r3 reuse) as an argument; the function takes none. */
        game_dispose_from_old_map();
        game_precache_new_map(options.map_name, 1);
        game_unload();
        main_new_map(&options);

        for ( i = 0; i < player_spawn_count; ++i )
            player_profile_save_last_level_played(i);
    }

    main_globals.map_change_load_timer = 0;
}
