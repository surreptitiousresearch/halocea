#include <stdint.h>
#include "headers/blam_data_globals.h"
/* attract_mode_should_start @ 0x83784BB8 — decide whether the idle attract-mode demo
 * should kick in: only at the main menu (no precache, not networked, no bink), once the
 * time since the last input event exceeds the threshold. Also fades the menu music in/out
 * as the idle timer crosses ~73.5s, and forces start past ~75s (or on immediate-start). */

extern uint8_t cache_files_precache_in_progress(void);
extern int16_t cache_files_precache_map_status(float *progress);
extern void cache_files_precache_map_end(void);
extern uint8_t main_menu_screen_is_active(void);
extern uint8_t network_game_is_active(void);
extern uint8_t bink_playback_active(void);
extern uint32_t system_milliseconds(void);
extern uint32_t event_manager_time_of_last_event(void);
extern uint8_t ui_main_menu_music_active(void);
extern void ui_start_main_menu_music(void);
extern void ui_stop_main_menu_music(void);


uint8_t attract_mode_should_start(void)
{
    unsigned int now;
    unsigned int last_event;
    unsigned int reference;
    unsigned int idle;
    float precache_progress;

    if ( cache_files_precache_in_progress() && cache_files_precache_map_status(&precache_progress) == 1 )
        cache_files_precache_map_end();

    if ( !main_menu_screen_is_active()
      || cache_files_precache_in_progress()
      || network_game_is_active()
      || bink_playback_active() )
    {
        return 0;
    }

    now = system_milliseconds();
    last_event = event_manager_time_of_last_event();
    reference = (attract_mode_countdown_timer <= last_event) ? last_event : attract_mode_countdown_timer;
    idle = now - reference;

    if ( idle < 0x11F1C )
    {
        if ( !ui_main_menu_music_active() )
            ui_start_main_menu_music();
    }
    else if ( ui_main_menu_music_active() )
    {
        ui_stop_main_menu_music();
    }

    if ( idle < 0x124F8 && !attract_mode_immediate_start )
        return 0;
    return 1;
}
