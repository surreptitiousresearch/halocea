/* player_effect_update @ 0x83738570 — for every local player that has no live unit (not in the game, or
 * its unit object is gone), clear that player's screen/camera effect state and stop any controller
 * vibration. Players with a live unit keep their effects (updated elsewhere).
 *
 * Player datum stride is 512 bytes; +52 holds the unit object index. The n[57]=0 store is redundant
 * with the following 236-byte clear but is preserved from the decompiler. */

#include <stdint.h>
#include "headers/player_effect_globals.h"
#include "headers/player_datum.h"
#include "headers/player_effect_datum.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void *memset(void *dst, int c, unsigned int n);
extern int16_t local_player_get_next(int16_t local_player_index);
extern int local_player_get_player_index(int16_t local_player_index);
extern void vibrate_player_clear(int16_t local_player_index);

void player_effect_update(void)
{
    int16_t local_player;

    for ( local_player = local_player_get_next(-1); local_player != -1;
          local_player = local_player_get_next(local_player) )
    {
        int player_index = local_player_get_player_index(local_player);
        if ( player_index == -1
          || DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index == -1 )
        {
            player_effect_datum *effect = &player_effect_globals->local_player_effect_data[local_player];
            *(int *)effect->damage_indicator_ticks = 0;  /* 4-byte store clearing all four tick counters (redundant with the memset, preserved) */
            memset(effect, 0, sizeof(player_effect_datum));
            vibrate_player_clear(local_player);
        }
    }
}
