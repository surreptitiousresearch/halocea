/* main_skip_private @ 0x8368A1B0 — fast-forward a cutscene by stepping game time at
 * full speed for the requested number of ticks. */
#include <stdint.h>
#include "headers/main_globals.h"
#include "headers/blam_data_globals.h"

extern uint8_t cinematic_in_progress(void);
extern float game_time_get_speed(void);
extern void game_time_set_speed(float speed);
extern void game_time_update(float time_delta_sec);

void main_skip_private(void)
{
    if ( main_globals.skip_ticks && cinematic_in_progress() )
    {
        float saved_speed = game_time_get_speed();
        game_time_set_speed(1.0f);
        halo_is_skip_private = 1;
        while ( main_globals.skip_ticks > 0 )
        {
            main_globals.skip_ticks = main_globals.skip_ticks - 1;
            game_time_update(1.0f / 30.0f);
        }
        halo_is_skip_private = 0;
        game_time_set_speed(saved_speed);
    }
    main_globals.skip_ticks = 0;
    main_globals.cutscene_skip = 0;
}
