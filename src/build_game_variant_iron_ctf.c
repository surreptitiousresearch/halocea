/* build_game_variant_iron_ctf @0x8374FAA0 — "Iron CTF" variant: ctf engine (1), double health (2.0),
 * 450-tick respawn, flag-must-reset, vehicle sets 0x80080000. */

#include "headers/game_variant.h"
#include <string.h>
#include "headers/game_engine_type.h"
#include "headers/game_engine_weapons.h"
#include "headers/game_engine_vehicles.h"
#include "headers/game_engine_universal_variant_flags.h"

game_variant * build_game_variant_iron_ctf(game_variant *result)
{
    result->human_readable_game_description[0] = 0;
    memset(&result->human_readable_game_description[1], 0, 0x96u);

    int flags = result->universal_variant.flags;
    result->universal_variant.lives = 0;
    result->universal_variant.odd_man_out = 0;
    result->universal_variant.respawn_time_growth = 0;
    result->universal_variant.vehicles_respawn = 0;
    result->universal_variant.friendly_fire_penalty = 0;
    result->game_engine_index = game_engine_ctf;
    result->universal_variant.flags = (flags & (~((1u << NUMBER_OF_GAME_ENGINE_UNIVERSAL_VARIANT_FLAGS) - 1u) | (1u << _motion_tracker_no_enemies_bit))) | (1u << _allow_motion_tracker_bit) | (1u << _allow_friendly_navpoints_bit) | (1u << _game_mode_classic_bit);
    result->universal_variant.health = 2.0f;
    result->universal_variant.goal_radar = 1;
    result->universal_variant.respawn_time = 450;
    result->universal_variant.score_to_win = 3;
    result->universal_variant.suicide_penalty = 150;
    result->universal_variant.teams = 1;
    result->universal_variant.weapon_set = _game_engine_weapons_classic;
    result->universal_variant.friendly_fire = 1;
    result->universal_variant.team_autobalance = 0;
    result->universal_variant.time_limit = 0;
    result->universal_variant.vehicle_set.value = 0;
    result->universal_variant.vehicle_set.__s1.preset = _game_engine_vehicles_custom;
    result->universal_variant.vehicle_set.__s1.scorpions = 1;
    result->universal_variant.vehicle_set2.value = 0;
    result->universal_variant.vehicle_set2.__s1.preset = _game_engine_vehicles_custom;
    result->universal_variant.vehicle_set2.__s1.scorpions = 1;
    result->game_engine_variant.ctf.assault = 0;
    result->game_engine_variant.ctf.flag_at_home_to_score = 0;
    result->game_engine_variant.ctf.flag_must_reset = 1;
    result->game_engine_variant.ctf.reset_on_capture = 0;
    result->game_engine_variant.ctf.single_flag_time = 0;
    result->flags = 1;
    return result;
}
