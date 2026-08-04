/* ctf_player_changed_team @0x838071A0 — clears the player's CTF stat slot at word offset 100
 * (statistics.multiplayer_statistics.ctf_statistics.flag_scores = ctf_statistics.flag_scores, byte +200) when they change teams. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern void *datum_try_and_get(const data_array *data, int index);

void ctf_player_changed_team(int player_index, uint8_t new_team)
{
    player_datum *player = datum_try_and_get(player_data, player_index);
    if ( player )
        player->statistics.multiplayer_statistics.ctf_statistics.flag_scores = 0;  /* ctf_statistics.flag_scores */
}
