#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/render_constants.h"
/* main_get_window_count @0x83689D60 — number of player viewports to render this frame: forced to 1 for
 * single-screen, cinematic or coop; otherwise the local player count clamped to 1..2. */

extern uint8_t game_engine_force_single_screen(void);
extern uint8_t cinematic_in_progress(void);
extern int16_t local_player_count(void);

int16_t main_get_window_count(void)
{
    if ( game_engine_force_single_screen() || cinematic_in_progress() || hcex_coop_local_player_index >= 0 )
        return 1;
    if ( local_player_count() < 1 )
        return 1;
    if ( local_player_count() <= MAXIMUM_WINDOWS )
        return local_player_count();
    return MAXIMUM_WINDOWS;
}
