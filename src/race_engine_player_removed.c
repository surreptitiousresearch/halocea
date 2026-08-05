/* race_engine_player_removed @0x8382B570 — as the multiplayer server (game_engine_get_variant()->
 * game_engine_variant, viewed through .ctf.single_flag_time at absolute offset 0x80, holds 2 here — the
 * mode selector, not a real CTF field), credits the leaving player's race score onto their team's
 * persistent total before recomputing team scores. The player's race score is the lap count,
 * statistics.multiplayer_statistics.race_statistics.laps (__int16 @+0xC6), confirmed via
 * disasm_range(0x8382B570,0x8382B5D0). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/game_variant.h"
#include "headers/race_globals.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"

extern int16_t game_connection(void);
extern void *datum_try_and_get(const data_array *data, int index);
extern game_variant *game_engine_get_variant(void);
extern void race_update_team_score(void);

void race_engine_player_removed(int player_index)
{
    if ( game_connection() == _game_connection_network_server )
    {
        player_datum *player = datum_try_and_get(player_data, player_index);
        if ( player )
        {
            if ( game_engine_get_variant()->game_engine_variant.ctf.single_flag_time == 2 )
                race_globals.persistent_team_score[player->team_index] += player->statistics.multiplayer_statistics.race_statistics.laps;
        }

        race_update_team_score();
    }
}
