/* player_force_kill @0x836A87D0 — forcibly kill a player's unit. If the player is attached to a live
 * unit, records a suicide statistic (when scoring is enabled), optionally sets a respawn timer, zeroes
 * the unit's body vitality, depletes its body, and runs the forced-kill / player-died bookkeeping. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"


extern uint8_t game_engine_can_score(void);
extern void game_statistics_record_kill(int object_index, int owner_player_index, int owner_object_index, int16_t owner_team_index);
extern void object_deplete_body(int object_index);
extern void unit_handle_forced_player_kill(int player_index);
extern void player_died(int player_index);

void player_force_kill(int player_index, int respawn_time)
{
    if (player_index != -1)
    {
        player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
        int unit_index = player->unit_index;
        if (unit_index != -1)
        {
            if (game_engine_can_score())
                game_statistics_record_kill(player->unit_index, -1, -1, -1);
            if (respawn_time)
                player->respawn_timer = respawn_time;

            object_header_datum *header =
                DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index);
            header->datum->object.body_vitality = 0.0;
            object_deplete_body(unit_index);
            unit_handle_forced_player_kill(player_index);
            player_died(player_index);
        }
    }
}
