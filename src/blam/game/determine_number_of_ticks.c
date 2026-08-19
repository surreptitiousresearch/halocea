/* determine_number_of_ticks @ 0x8369AA68 — convert elapsed real seconds into a whole
 * number of 30Hz game ticks, banking the remainder in game_time leftover. Network and
 * co-op modes force fixed tick counts. `is_peek` (nonzero) suppresses updating the leftover.
 *
 * Attested against disasm (2026-07-28): the function takes exactly two args — a float in
 * f1 (feeds fadds directly, no frsp => declared float, not double) and an 8-bit peek flag
 * in r4 (clrlwi r30,24 @0x8369AB80 gates the leftover write). The prior reconstruction's
 * phantom third arg (`peek_only` in r5) is never referenced in the body; removed. */
#include <stdint.h>
#include "headers/game_time_globals.h"

#include <math.h>
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"

extern int16_t game_connection(void);

int determine_number_of_ticks(float time_delta_sec, uint8_t is_peek)
{
    float speed;
    float total_sec;
    float ticks_per_sec;
    float whole_ticks;
    int result;

    if ( game_connection() == _game_connection_network_client || game_connection() == _game_connection_network_server )
        speed = 1.0f;
    else
        speed = game_time_globals->speed;

    total_sec = game_time_globals->leftover_time_sec + time_delta_sec;
    ticks_per_sec = speed * 30.0f;
    whole_ticks = (float)floor(total_sec * ticks_per_sec);

    if ( hcex_force_one_tick || game_connection() == _game_connection_film_playback || hcex_allow_saved_film_recoring )
        return 1;
    if ( hcex_coop_local_player_index != -1 && !hcex_coop_online_test_local )
        return hcex_coop_nticks_to_apply;

    result = (int)(whole_ticks <= 1000.0f ? whole_ticks : 1000.0f);

    if ( ticks_per_sec > 0.0f && !is_peek )
    {
        game_time_globals->leftover_time_sec = total_sec - (whole_ticks / ticks_per_sec);
        if ( game_time_globals->leftover_time_sec < 0.0f )
            game_time_globals->leftover_time_sec = 0.0f;
    }
    return result;
}
