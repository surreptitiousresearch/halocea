/* build_game_variant_slayer_standard @0x8374AF90 — "Slayer" standard-lobby variant: slayer engine (2),
 * 25 kills to win, 10-minute (36000 tick) time limit, 150-tick suicide penalty, default weapon set,
 * vehicle sets 0x09240000, vehicles respawn in 1800 ticks, standard flags mask (0xFFFFFE00 | 0x103). */

#include "headers/game_variant.h"
#include <string.h>
#include "headers/game_engine_type.h"
#include "headers/game_engine_weapons.h"
#include "headers/game_engine_vehicles.h"
#include "headers/game_engine_universal_variant_flags.h"

game_variant * build_game_variant_slayer_standard(game_variant *result)
{
    result->human_readable_game_description[0] = 0;
    memset(&result->human_readable_game_description[1], 0, 0x96u);

    result->universal_variant.lives = 0;
    result->universal_variant.respawn_time = 0;
    result->universal_variant.respawn_time_growth = 0;
    result->universal_variant.odd_man_out = 0;
    result->universal_variant.weapon_set = _game_engine_weapons_normal;
    result->universal_variant.health = 1.0f;
    result->universal_variant.vehicles_respawn = 1800;
    result->universal_variant.goal_radar = 0;
    result->game_engine_index = game_engine_slayer;
    result->universal_variant.suicide_penalty = 150;
    result->universal_variant.vehicle_set.value = 0;
    result->universal_variant.vehicle_set.__s1.preset = _game_engine_vehicles_default;
    result->universal_variant.vehicle_set.__s1.warthogs = 4;
    result->universal_variant.vehicle_set.__s1.ghosts = 4;
    result->universal_variant.vehicle_set.__s1.scorpions = 4;
    result->universal_variant.vehicle_set.__s1.rwarthogs = 4;
    result->universal_variant.vehicle_set2.value = 0;
    result->universal_variant.vehicle_set2.__s1.preset = _game_engine_vehicles_default;
    result->universal_variant.vehicle_set2.__s1.warthogs = 4;
    result->universal_variant.vehicle_set2.__s1.ghosts = 4;
    result->universal_variant.vehicle_set2.__s1.scorpions = 4;
    result->universal_variant.vehicle_set2.__s1.rwarthogs = 4;
    result->universal_variant.friendly_fire = 1;
    result->universal_variant.friendly_fire_penalty = 0;
    result->universal_variant.team_autobalance = 0;
    result->game_engine_variant.ctf.assault = 1;
    result->game_engine_variant.ctf.flag_must_reset = 0;
    result->game_engine_variant.ctf.reset_on_capture = 1;
    int flags = result->universal_variant.flags;
    result->universal_variant.score_to_win = 25;
    result->universal_variant.teams = 0;
    result->universal_variant.time_limit = 36000;
    result->universal_variant.flags = (flags & ~((1u << NUMBER_OF_GAME_ENGINE_UNIVERSAL_VARIANT_FLAGS) - 1u)) | (1u << _allow_motion_tracker_bit) | (1u << _allow_friendly_navpoints_bit) | (1u << _game_mode_standard);
    result->flags = 1;
    return result;
}
