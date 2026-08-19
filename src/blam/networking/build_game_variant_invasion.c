/* build_game_variant_invasion @0x8374F9C0 — "Invasion" variant: ctf engine (1) in assault mode with
 * 5 lives, instant respawn, 3 scores to win, weapon set 11, vehicle sets 0x28000000. */

#include "headers/game_variant.h"
#include <string.h>
#include "headers/game_engine_type.h"
#include "headers/game_engine_weapons.h"
#include "headers/game_engine_vehicles.h"
#include "headers/game_engine_universal_variant_flags.h"

game_variant * build_game_variant_invasion(game_variant *result)
{
    result->human_readable_game_description[0] = 0;
    memset(&result->human_readable_game_description[1], 0, 0x96u);

    int flags = result->universal_variant.flags;
    result->universal_variant.odd_man_out = 0;
    result->universal_variant.respawn_time = 0;
    result->universal_variant.respawn_time_growth = 0;
    result->universal_variant.vehicles_respawn = 0;
    result->universal_variant.friendly_fire_penalty = 0;
    result->game_engine_index = game_engine_ctf;
    result->universal_variant.flags = (flags & (~((1u << NUMBER_OF_GAME_ENGINE_UNIVERSAL_VARIANT_FLAGS) - 1u) | (1u << _motion_tracker_no_enemies))) | (1u << _allow_motion_tracker_bit) | (1u << _allow_friendly_navpoints_bit) | (1u << _game_mode_classic);
    result->universal_variant.health = 1.0f;
    result->universal_variant.goal_radar = 1;
    result->universal_variant.lives = 5;
    result->universal_variant.score_to_win = 3;
    result->universal_variant.suicide_penalty = 150;
    result->universal_variant.teams = 1;
    result->universal_variant.weapon_set = _game_engine_weapons_classic;
    result->universal_variant.vehicle_set.value = 0;
    result->universal_variant.vehicle_set.__s1.preset = _game_engine_vehicles_warthog;
    result->universal_variant.vehicle_set.__s1.warthogs = 4;
    result->universal_variant.vehicle_set2.value = 0;
    result->universal_variant.vehicle_set2.__s1.preset = _game_engine_vehicles_warthog;
    result->universal_variant.vehicle_set2.__s1.warthogs = 4;
    result->universal_variant.friendly_fire = 1;
    result->universal_variant.team_autobalance = 0;
    result->universal_variant.time_limit = 0;
    result->game_engine_variant.ctf.assault = 1;
    result->game_engine_variant.ctf.flag_at_home_to_score = 0;
    result->game_engine_variant.ctf.flag_must_reset = 0;
    result->game_engine_variant.ctf.reset_on_capture = 0;
    result->game_engine_variant.ctf.single_flag_time = 0;
    result->flags = 1;
    return result;
}
