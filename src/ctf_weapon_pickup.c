/* ctf_weapon_pickup @0x83807A78 — handle a player touching a CTF flag (the flag is a weapon object). Only acts
 * on a real player in a multiplayer session (game_connection == 2). If the flag belongs to a team other than
 * the toucher's, it is a capture-side pickup: once, when scoring is allowed and not in Assault, it bumps the
 * player's captures, plays the pickup jingle (team-relative), raises the flag-taken warning and announces it;
 * the flag object is then marked picked-up (flag 0x40) and the weapon is left in the world (returns 1). If the
 * flag is the toucher's own team's, it is a return/reset: under "flag must reset" a dropped flag just plays the
 * failure sound; otherwise a dropped flag that can score clears its warning, bumps the player's returns, and
 * announces/plays the return jingle, and the flag is reset to home. Returns 1 to leave the weapon placed, 0 to
 * consume it.
 *
 * Weapon object fields resolved to the DB weapon_datum struct (+184 owning team, +556 flags bit 0x40 =
 * already handled/dropped); player fields to player_datum (+32 team, +196 captures, +198 returns). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/ctf_globals.h"
#include "headers/game_variant.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_datum_flags.h"
#include "headers/player_datum.h"
#include "headers/game_connection.h"
#include "headers/multiplayer_team.h"
#include "headers/game_engine_multiplayer_sound.h"
#include "headers/ctf_message.h"
#include "headers/blam_data_globals.h"


extern int16_t game_connection(void);
extern uint8_t game_engine_can_score(void);
extern game_variant *game_engine_get_variant(void);
extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);
extern void game_show_score_you_ally_enemy(int player_index, int message_you, int message_ally, int message_enemy, int message_data, uint8_t should_replicate);
extern void ctf_flag_failure_sound(int player_index);
extern void weapon_reset_flag(int weapon_index);

uint8_t ctf_weapon_pickup(int weapon_index, int player_index)
{
    int result = 1;
    weapon_datum *weapon = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;

    if ( player_index == -1 || game_connection() != _game_connection_network_server )
        return result;

    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    if ( weapon->object.owner_team_index != player->team_index )   /* flag team != player team */
    {
        if ( (weapon->weapon.flags & (1u << _weapon_multiplayer_flag)) == 0 )
        {
            if ( game_engine_can_score() )
            {
                ++player->statistics.multiplayer_statistics.ctf_statistics.flag_grabs;  /* captures */
                if ( !game_engine_get_variant()->game_engine_variant.ctf.assault )
                {
                    game_engine_play_multiplayer_sound(player->team_index == _multiplayer_team_red
                            ? _multiplayer_sound_ctf_red_took_flag
                            : _multiplayer_sound_ctf_blue_took_flag, 1u);
                    __int16 flag_team = weapon->object.owner_team_index;
                    ctf_globals.flag_warning[flag_team] = 1;
                    ctf_globals.flag_warning_time_in_ticks[flag_team] = 0;
                    game_show_score_you_ally_enemy(player_index, -1, ctf_message_good_guys_have_the_flag, ctf_message_enemy_has_the_flag, player_index, 1u);
                }
            }
        }
        weapon->weapon.flags |= (1u << _weapon_multiplayer_flag);
        return result;
    }

    game_variant *variant = game_engine_get_variant();
    result = 0;
    int already_dropped = weapon->weapon.flags & (1u << _weapon_multiplayer_flag);

    if ( variant->game_engine_variant.ctf.flag_must_reset )
    {
        if ( already_dropped )
        {
            ctf_flag_failure_sound(player_index);
            return 0;
        }
        return result;
    }

    if ( already_dropped )
    {
        if ( game_engine_can_score() )
        {
            __int16 flag_team = weapon->object.owner_team_index;
            ctf_globals.flag_warning[flag_team] = 0;
            ctf_globals.flag_warning_time_in_ticks[flag_team] = 0;
            ++player->statistics.multiplayer_statistics.ctf_statistics.flag_returns;  /* returns */
            game_show_score_you_ally_enemy(player_index, ctf_message_you_returned_the_flag, ctf_message_good_guys_returned_the_flag, ctf_message_enemy_returned_the_flag, player_index, 1u);
            game_engine_play_multiplayer_sound(player->team_index == _multiplayer_team_red
                    ? _multiplayer_sound_ctf_red_returned_flag
                    : _multiplayer_sound_ctf_blue_returned_flag, 1u);
        }
    }

    weapon_reset_flag(weapon_index);
    return 0;
}
