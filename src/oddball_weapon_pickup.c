/* oddball_weapon_pickup @0x83817AF0 — oddball engine hook for a player picking up the ball weapon: in the
 * special ball modes (oddball_ball_type 1..2) just broadcast the you/ally/enemy pickup messages (31/32/33)
 * and allow the pickup. Otherwise allow it only when the player's unit doesn't already hold a weapon with
 * flag 3 (another ball), marking the ball in play (weapon flags dword +556 bit 6). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/game_variant.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_datum_flags.h"
#include "headers/oddball_message.h"
#include "headers/weapon_definition_flags.h"
#include "headers/blam_data_globals.h"


extern game_variant *game_engine_get_variant(void);
extern void game_show_score_you_ally_enemy(int player_index, int message_you, int message_ally, int message_enemy, int message_data, uint8_t should_replicate);
extern uint8_t unit_has_weapon_with_flag(int unit_index, int flag);

uint8_t oddball_weapon_pickup(int weapon_index, int player_index)
{
    weapon_datum *weapon = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;

    int ball_type = game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type;
    if ( ball_type > 0 && ball_type <= 2 )
    {
        game_show_score_you_ally_enemy(player_index, oddball_message_you_have_the_ball,
                oddball_message_ally_has_the_ball, oddball_message_enemy_has_the_ball, player_index, 0);
        return 1;
    }

    int unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;
    if ( unit_index == -1 )
        return 1;

    uint8_t allowed = unit_has_weapon_with_flag(unit_index, _weapon_must_be_readied_bit) == 0;
    if ( allowed )
        weapon->weapon.flags |= (1u << _weapon_multiplayer_flag);
    return allowed;
}
