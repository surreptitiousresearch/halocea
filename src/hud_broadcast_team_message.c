/* hud_broadcast_team_message @0x836A59E8 — while a game engine is running, print `message` to the HUD of
 * every local player whose team matches the victim player's team. */

#include <wchar.h>
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern uint8_t game_engine_running(void);
extern int local_player_get_player_index(int16_t local_player_index);
extern void hud_print_message(int16_t local_player_index, const wchar_t *text);

void hud_broadcast_team_message(int victim_player_index, const wchar_t *message)
{
    if ( !game_engine_running() )
        return;

    for ( int16_t local_player_index = 0; local_player_index < 2; ++local_player_index )
    {
        int player_index = local_player_get_player_index(local_player_index);
        if ( player_index == -1 )
            continue;

        player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
        player_datum *victim = DATA_ARRAY_ELEMENT(player_data, player_datum, victim_player_index);
        if ( player->team_index == victim->team_index )
            hud_print_message(local_player_index, message);
    }
}
