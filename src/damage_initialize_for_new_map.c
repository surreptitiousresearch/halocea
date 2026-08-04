#include "headers/blam_data_globals.h"
/* damage_initialize_for_new_map @0x836B1148 — reset the per-map damage feedback state (multiplayer hit-sound
 * throttle and last-damaged-object marker). */

extern int last_damaged_object_index; /* last-damaged object index (no symbol in DB) */

void damage_initialize_for_new_map(void)
{
    g_last_multiplayer_hit_sound_time = 0;
    last_damaged_object_index = -1;
}
