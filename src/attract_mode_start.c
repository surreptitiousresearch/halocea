#include <stdint.h>
#include "headers/blam_data_globals.h"
/* attract_mode_start @0x83784F8C — begin attract-mode (idle demo) playback: pick a random movie 0-2
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
        short roll = seed_random_range(get_global_local_random_seed_address(), 0, 3);
        if ( roll < 0 )
            movie = 0;
        else if ( roll > 2 )
            movie = 2;
        else
            movie = roll;
    }
    while ( movie == last_attract_movie );

    last_attract_movie = movie;
    attract_mode_immediate_start = 0;
    ui_stop_main_menu_music();
    bink_playback_start(attract_mode_get_localized_movie_path(movie), 0x2Eu);

    /* return value is residue (bink_playback_active / system_milliseconds), ignored by caller: void */
    if ( !bink_playback_active() )
        attract_mode_countdown_timer = system_milliseconds();
}
