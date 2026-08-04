/* build_game_variant_team_king_standard @0x8374B070 — "standard" (matchmaking-rules) Team King variant:
 * King (engine 4) with teams, autobalance and a 300-tick friendly-fire penalty, a moving hill, standard
 * vehicle set (0x09240000) respawning in 1800 ticks, and a 10-minute time limit. */

#include "headers/game_variant.h"
#include <string.h>
#include "headers/game_engine_type.h"
#include "headers/game_engine_weapons.h"
#include "headers/game_engine_vehicles.h"
#include "headers/game_engine_universal_variant_flags.h"

game_variant * build_game_variant_team_king_standard(game_variant *result)
{
    result->human_readable_game_description[0] = 0;
    memset(&result->human_readable_game_description[1], 0, 0x96u);

    result->universal_variant.lives = 0;
    result->universal_variant.respawn_time_growth = 0;
    result->game_engine_index = game_engine_king;
    result->universal_variant.odd_man_out = 0;
    result->universal_variant.health = 1.0f;
    result->universal_variant.weapon_set = _game_engine_weapons_normal;
    result->universal_variant.vehicles_respawn = 1800;
    result->universal_variant.goal_radar = 1;
    result->universal_variant.respawn_time = 300;
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
    result->universal_variant.friendly_fire_penalty = 300;
    result->universal_variant.team_autobalance = 1;
    result->game_engine_variant.ctf.assault = 1;
    result->universal_variant.score_to_win = 2;
    result->universal_variant.teams = 1;
    int flags = result->universal_variant.flags;
    result->universal_variant.time_limit = 36000;
    result->flags = 1;
    result->universal_variant.flags = (flags & ~((1u << NUMBER_OF_GAME_ENGINE_UNIVERSAL_VARIANT_FLAGS) - 1u)) | (1u << _allow_motion_tracker_bit) | (1u << _allow_friendly_navpoints_bit) | (1u << _game_mode_standard_bit);
    return result;
}
