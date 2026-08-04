/* cinematic_initialize_for_new_map @0x83690E58 — clear the cinematic globals header (letterbox state and
 * flags), mark all four active title slots inactive (-1), and reset the in-cinematic music gain. */

#include "headers/cinematic_globals.h"
#include "headers/blam_data_globals.h"


void cinematic_initialize_for_new_map(void)
{
    /* recovered: int *header walk over 7 dwords -> named fields. header[0..2] are the letterbox fields and
     * flags (zeroed here); header[3..6] are active_titles[0..3], zeroed then immediately set to -1 below. */
    cinematic_globals->letter_box_amount = 0.0f;
    cinematic_globals->letter_box_last_game_time = 0;
    cinematic_globals->letter_box = 0;
    cinematic_globals->cinematic_in_progress = 0;
    cinematic_globals->cinematic_skip_in_progress = 0;
    cinematic_globals->cinematic_suppress_bsp_object_creation = 0;

    cinematic_globals->active_titles[0].title_index = -1;
    cinematic_globals->active_titles[0].title_timer = -1;
    cinematic_globals->active_titles[1].title_index = -1;
    cinematic_globals->active_titles[1].title_timer = -1;
    cinematic_globals->active_titles[2].title_index = -1;
    cinematic_globals->active_titles[2].title_timer = -1;
    cinematic_globals->active_titles[3].title_index = -1;
    cinematic_globals->active_titles[3].title_timer = -1;

    music_gain_in_profile = -1.0f;
}
