/* build_game_variant_rockets @0x8374ECD8 — default "Rockets" variant: Slayer (engine 2) to 25 kills with
 * the rockets weapon set (6), instant respawn, and universal flag bits 0xA2 (the compiled
 * __ROL4__(81,1)&0x3F | ... form). */

#include "headers/game_variant.h"
#include <string.h>
#include "headers/game_engine_type.h"
#include "headers/game_engine_weapons.h"
#include "headers/game_engine_vehicles.h"
#include "headers/game_engine_universal_variant_flags.h"

game_variant * build_game_variant_rockets(game_variant *result)
{
    result->human_readable_game_description[0] = 0;
    memset(&result->human_readable_game_description[1], 0, 0x96u);

    int flags = result->universal_variant.flags;
    result->universal_variant.lives = 0;
    result->universal_variant.odd_man_out = 0;
    result->universal_variant.respawn_time = 0;
    result->universal_variant.respawn_time_growth = 0;
    result->universal_variant.teams = 0;
    result->universal_variant.goal_radar = 1;
    result->universal_variant.flags = (flags & (~((1u << NUMBER_OF_GAME_ENGINE_UNIVERSAL_VARIANT_FLAGS) - 1u) | (1u << _motion_tracker_no_enemies))) | (1u << _allow_friendly_navpoints_bit) | (1u << _generic_starting_equipment_bit) | (1u << _game_mode_classic);   /* __ROL4__(81,1)&0x3F | 0x80 */
    result->universal_variant.health = 1.0f;
    result->game_engine_index = game_engine_slayer;
    result->universal_variant.score_to_win = 25;
    result->universal_variant.suicide_penalty = 300;
    result->universal_variant.weapon_set = _game_engine_weapons_rocket_launchers;
    result->universal_variant.vehicle_set.value = 0;
    result->universal_variant.vehicle_set.__s1.preset = _game_engine_vehicles_warthog;
    result->universal_variant.vehicle_set.__s1.warthogs = 4;
    result->universal_variant.vehicle_set2.value = 0;
    result->universal_variant.vehicle_set2.__s1.preset = _game_engine_vehicles_warthog;
    result->universal_variant.vehicle_set2.__s1.warthogs = 4;
    result->universal_variant.vehicles_respawn = 0;
    result->universal_variant.friendly_fire = 1;
    result->universal_variant.friendly_fire_penalty = 0;
    result->universal_variant.team_autobalance = 0;
    result->universal_variant.time_limit = 0;
    result->game_engine_variant.ctf.assault = 0;
    result->game_engine_variant.ctf.reset_on_capture = 0;
    result->game_engine_variant.ctf.flag_must_reset = 0;
    result->flags = 1;
    return result;
}
