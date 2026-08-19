#include <stdint.h>
#include "headers/bink_movie.h"
#include "headers/bink_playback_flags.h"
#include "headers/blam_data_globals.h"
/* attract_mode_start @0x83784F38 — begin attract-mode (idle demo) playback: pick a random movie 0-2
 * different from the last one shown, stop the menu music, and start its localized Bink movie. If
 * playback fails to start, arm the retry countdown timer. */

extern uint32_t *get_global_local_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern void ui_stop_main_menu_music(void);
extern char *attract_mode_get_localized_movie_path(int16_t movie);
extern void bink_playback_start(const char *full_pathname, unsigned int flags);
extern uint8_t bink_playback_active(void);
extern uint32_t system_milliseconds(void);

void attract_mode_start(void)
{
    short movie;
    do
    {
        short roll = seed_random_range(get_global_local_random_seed_address(), 0, NUMBER_OF_ATTRACT_MODE_MOVIES);
        if ( roll < 0 )
            movie = _bink_attract1_movie;
        else if ( roll > _bink_attract3_movie )
            movie = _bink_attract3_movie;
        else
            movie = roll;
    }
    while ( movie == last_attract_movie );

    last_attract_movie = movie;
    attract_mode_immediate_start = 0;
    ui_stop_main_menu_music();
    bink_playback_start(attract_mode_get_localized_movie_path(movie),
                        (1u << _bink_playback_button_click_stops_movie_bit)
                      | (1u << _bink_playback_prevent_events_to_ui_bit)
                      | (1u << _bink_playback_dont_render_ui_bit)
                      | (1u << _bink_playback_return_to_main_menu_when_finished_bit));

    /* return value is residue (bink_playback_active / system_milliseconds), ignored by caller: void */
    if ( !bink_playback_active() )
        attract_mode_countdown_timer = system_milliseconds();
}
