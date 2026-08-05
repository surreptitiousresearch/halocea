/* race_player_changed_team @0x8382B410 — (server only) handle a player switching teams during a race:
 * under team scoring (the race view of the variant word race files read via ctf.single_flag_time == 2),
 * bank the player's current lap total into the persistent score of the team that earned it (the player's
 * recorded team at player+32; if that already equals the new team, the OTHER of the two teams — decoded
 * numerically from the compiled -1-((new_team==1)-2) form). Then reset the player's race words (lap
 * progress 98/99/100), restamp the join time (player dword 34), clear the lap bit vector entry, and
 * recompute the team score. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/race_globals.h"
#include "headers/game_variant.h"
#include "headers/player_datum.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"


extern int16_t game_connection(void);
extern void *datum_try_and_get(const data_array *data, int index);
extern game_variant *game_engine_get_variant(void);
extern int game_time_get(void);
extern void race_update_team_score(void);

void race_player_changed_team(int player_index, uint8_t new_team)
{
    if ( game_connection() != _game_connection_network_server )
        return;

    player_datum *player = datum_try_and_get(player_data, player_index);
    if ( player )
    {
        if ( game_engine_get_variant()->game_engine_variant.ctf.single_flag_time == 2 )
        {
            int scoring_team = player->team_index;
            if ( new_team == scoring_team )
                scoring_team = new_team == 1 ? 0 : 1;
            race_globals.persistent_team_score[scoring_team] +=
                player->statistics.multiplayer_statistics.ctf_statistics.flag_returns;
        }
        /* race lap words overlay the ctf-statistics union (98/99/100 = flag_grabs/returns/scores). */
        player->statistics.multiplayer_statistics.ctf_statistics.flag_grabs = 0;
        player->statistics.multiplayer_statistics.ctf_statistics.flag_returns = 0;
        player->statistics.multiplayer_statistics.ctf_statistics.flag_scores = 0;
        player->multiplayer.special = game_time_get();
        race_globals.lap_bit_vector[(uint16_t)player_index] = 0;
    }
    race_update_team_score();
}
