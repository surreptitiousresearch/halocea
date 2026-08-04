/* ctf_engine_player_update @0x83807320 — per-frame Capture-the-Flag update for one player (sibling of
 * king_engine_player_update / oddball_engine_player_update). Carrying the flag strips active camo. On the host,
 * if the player's unit is holding a flag weapon and is within one world unit of that flag's home position, and
 * scoring is allowed (in flag-at-home scoring the player's own flag must not be taken, except in single-flag
 * CTF), the capture is scored: the team score and the player's capture count are bumped, the score/state is
 * replicated, capture sounds and score banners are shown, the flag is dropped (locally and, if the unit is not
 * already flagged, over the network) and reset. A capture attempt with the own flag away plays the failure sound.
 *
 * Object/player fields resolved to the DB unit_datum/weapon_datum/player_datum structs (unit weapon slot
 * @754, held weapon @760+4*slot, position @92, object flags @4/@556; player unit index @52, team @32,
 * capture count @200). DEVIATION: the
 * `unit != -92` guard is Hex-Rays' rendering of an `addic. r31,0x5C` test (unit+92 != 0) — vestigial, always
 * true for a real object. Own-flag-home check is `!(own_flag->flags & 0x40)` via the `~x >> 6 & 1` idiom. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/ctf_globals.h"
#include "headers/game_variant.h"
#include "headers/scenario_netgame_flag.h"
#include "headers/message_delta_processor_mode.h"
#include "headers/player_datum.h"
#include "headers/object_datum.h"
#include "headers/unit_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_datum_flags.h"
#include "headers/game_connection.h"
#include "headers/game_engine_multiplayer_sound.h"
#include "headers/multiplayer_team.h"
#include "headers/ctf_message.h"
#include "headers/blam_data_globals.h"


extern uint8_t game_engine_player_has_flag(int player_index);
extern void game_engine_player_depower_active_camo(int player_index);
extern int16_t game_connection(void);
extern uint8_t game_engine_can_score(void);
extern uint8_t weapon_is_flag(int weapon_index);
extern game_variant *game_engine_get_variant(void);
extern void game_engine_replicate_score_and_state_to_network(message_delta_processor_mode mode, int machine_index);
extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);
extern void game_show_score_you_ally_enemy(int player_index, int message_you, int message_ally, int message_enemy, int message_data, uint8_t should_replicate);
extern void unit_drop_current_weapon_to_network(int unit_index, uint8_t immediate);
extern uint8_t unit_drop_current_weapon(int unit_index, uint8_t immediate);
extern void weapon_reset_flag(int weapon_index);
extern void ctf_flag_failure_sound(int player_index);


void ctf_engine_player_update(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    if ( game_engine_player_has_flag(player_index) )
        game_engine_player_depower_active_camo(player_index);

    if ( game_connection() != _game_connection_network_server || player->unit_index == -1 )
        return;

    unit_datum *unit = (unit_datum *)(DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player->unit_index)->datum);
    __int16 weapon_slot = unit->unit.current_weapon_index;
    if ( weapon_slot == -1 )
        return;

    int flag_object_index = unit->unit.weapon_object_indices[weapon_slot];
    if ( flag_object_index == -1 || !game_engine_can_score() || !weapon_is_flag(flag_object_index) )
        return;

    int team = player->team_index;
    unsigned __int8 at_capture_point = 0;
    if ( (char *)unit != (char *)-92 )  /* vestigial addic. guard (unit+92 != 0) */
    {
        scenario_netgame_flag *flag = ctf_globals.flags[team];
        if ( flag )
        {
            float dx = flag->position.n[0] - unit->object.position.n[0];
            float dy = flag->position.n[1] - unit->object.position.n[1];
            float dz = flag->position.n[2] - unit->object.position.n[2];
            at_capture_point = dy * dy + (dx * dx + dz * dz) < 1.0f;
        }
    }

    if ( !at_capture_point )
        return;

    unsigned int can_score = 1;
    if ( game_engine_get_variant()->game_engine_variant.ctf.flag_at_home_to_score
            && !game_engine_get_variant()->game_engine_variant.ctf.single_flag_time )
    {
        weapon_datum *own_flag = (weapon_datum *)(DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, ctf_globals.weapon_index[team])->datum);
        can_score = (~own_flag->weapon.flags >> _weapon_multiplayer_flag) & 1;
    }

    if ( !can_score )
    {
        ctf_flag_failure_sound(player_index);
        return;
    }

    ++ctf_globals.score[team];
    ++player->statistics.multiplayer_statistics.ctf_statistics.flag_scores;  /* captures */
    if ( game_connection() == _game_connection_network_server )
        game_engine_replicate_score_and_state_to_network(_message_delta_mode_incremental, -1);
    game_engine_play_multiplayer_sound(
        team == _multiplayer_team_red ? _multiplayer_sound_ctf_red_captured_flag
                                      : _multiplayer_sound_ctf_blue_captured_flag, 1);
    game_show_score_you_ally_enemy(player_index, ctf_message_you_scored, ctf_message_good_guys_scored, ctf_message_enemy_scored, player_index, 1);

    int unit_index = player->unit_index;
    if ( !((DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum))->object.datum_role )
        unit_drop_current_weapon_to_network(unit_index, 1);
    unit_drop_current_weapon(unit_index, 1);
    weapon_reset_flag(flag_object_index);
    game_engine_get_variant();
}
