/* build_game_variant_king @0x8374F4B0 — populate `result` with the default "King of the Hill" variant
 * (game engine 4): the whole variant block is zeroed first (description[0] = 0 plus a 0x96-byte memset —
 * covering through the engine-specific union and flags), then radar on, 150-tick respawn, score-to-win 2
 * (minutes on the hill), suicide penalty 150, all-weapons set, friendly fire on, all vehicles, universal
 * flags (0 after the clear) masked 0xFFFFFE40 and OR'd with 0x83, and non-moving hill (ctf.assault = 0). */

#include "headers/game_variant.h"
#include <string.h>
#include "headers/game_engine_type.h"
#include "headers/game_engine_weapons.h"
#include "headers/game_engine_vehicles.h"
#include "headers/game_engine_universal_variant_flags.h"

game_variant * build_game_variant_king(game_variant *result)
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
    result->universal_variant.flags = (flags & (~((1u << NUMBER_OF_GAME_ENGINE_UNIVERSAL_VARIANT_FLAGS) - 1u) | (1u << _motion_tracker_no_enemies_bit))) | (1u << _allow_motion_tracker_bit) | (1u << _allow_friendly_navpoints_bit) | (1u << _game_mode_classic_bit);
    result->universal_variant.health = 1.0f;
    result->universal_variant.respawn_time = 150;
    result->game_engine_index = game_engine_king;
    result->universal_variant.score_to_win = 2;
    result->universal_variant.suicide_penalty = 150;
    result->universal_variant.weapon_set = _game_engine_weapons_classic;
    result->universal_variant.vehicle_set.value = 0;
    result->universal_variant.vehicle_set.__s1.preset = _game_engine_vehicles_warthog;
    result->universal_variant.vehicle_set.__s1.warthogs = 4;
    result->universal_variant.vehicle_set2.value = 0;
    result->universal_variant.vehicle_set2.__s1.preset = _game_engine_vehicles_warthog;
    result->universal_variant.vehicle_set2.__s1.warthogs = 4;
    result->universal_variant.friendly_fire = 1;
    result->universal_variant.friendly_fire_penalty = 0;
    result->universal_variant.team_autobalance = 0;
    result->universal_variant.time_limit = 0;
    result->game_engine_variant.ctf.assault = 0;
    result->flags = 1;
    return result;
}
