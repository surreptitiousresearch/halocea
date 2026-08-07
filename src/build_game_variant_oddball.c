/* build_game_variant_oddball @0x8374EF68 — default "Oddball" variant: oddball engine (3), no teams,
 * 150-tick instant respawn, 2 points to win, weapon set 11, vehicle set 0x10000000, normal ball type with
 * no traits, resets on capture (goal-position semantics reused from the ctf view). */

#include "headers/game_variant.h"
#include <string.h>
#include "headers/game_engine_type.h"
#include "headers/game_engine_weapons.h"
#include "headers/game_engine_vehicles.h"
#include "headers/game_engine_universal_variant_flags.h"

game_variant * build_game_variant_oddball(game_variant *result)
{
    result->human_readable_game_description[0] = 0;
    memset(&result->human_readable_game_description[1], 0, 0x96u);

    int flags = result->universal_variant.flags;
    result->universal_variant.lives = 0;
    result->universal_variant.odd_man_out = 0;
    result->universal_variant.respawn_time_growth = 0;
    result->universal_variant.teams = 0;
    result->universal_variant.vehicles_respawn = 0;
    result->universal_variant.goal_radar = 1;
    result->universal_variant.flags = (flags & (~((1u << NUMBER_OF_GAME_ENGINE_UNIVERSAL_VARIANT_FLAGS) - 1u) | (1u << _motion_tracker_no_enemies))) | (1u << _allow_motion_tracker_bit) | (1u << _allow_friendly_navpoints_bit) | (1u << _game_mode_classic);
    result->universal_variant.health = 1.0f;
    result->universal_variant.respawn_time = 150;
    result->game_engine_index = game_engine_oddball;
    result->universal_variant.score_to_win = 2;
    result->universal_variant.suicide_penalty = 150;
    result->universal_variant.weapon_set = _game_engine_weapons_classic;
    result->universal_variant.friendly_fire = 1;
    result->universal_variant.friendly_fire_penalty = 0;
    result->universal_variant.team_autobalance = 0;
    result->universal_variant.time_limit = 0;
    result->universal_variant.vehicle_set.value = 0;
    result->universal_variant.vehicle_set.__s1.preset = _game_engine_vehicles_none;
    result->universal_variant.vehicle_set2.value = 0;
    result->universal_variant.vehicle_set2.__s1.preset = _game_engine_vehicles_none;
    result->game_engine_variant.oddball.ball_spawn_count = 1;
    result->game_engine_variant.ctf.reset_on_capture = 1;
    result->game_engine_variant.ctf.assault = 0;
    result->game_engine_variant.oddball.oddball_ball_type = _oddball_normal;
    result->game_engine_variant.oddball.trait_with_ball = _game_trait_none;
    result->game_engine_variant.oddball.trait_without_ball = _game_trait_none;
    result->game_engine_variant.ctf.single_flag_time = 0;
    result->flags = 1;
    return result;
}
