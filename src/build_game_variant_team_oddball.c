/* build_game_variant_team_oddball @0x8374F064 — populate `result` with the default "Team Oddball" game
 * variant settings (game engine 3): 1 life pool disabled, radar on, 300-tick respawn, score-to-win 2,
 * suicide penalty 150, classic weapon set, friendly fire on, vehicles preset = none, one spawning ball with
 * no traits. universal_variant.flags: config bits 0-5 reset then allow_motion_tracker | allow_friendly_
 * navpoints | generic_starting_equipment set, then game_mode forced to classic (bit 7 set, bit 8 cleared).
 *
 * DEVIATION: the decompiler rendered the traits clear as a raw 8-byte union write
 * (`*((_QWORD*)&result->game_engine_variant.terminator + 1) = 0`); this spans exactly
 * oddball_variant.trait_with_ball/trait_without_ball (0x08..0x10 of the union) — restored as the two
 * named field assignments. */

#include "headers/game_variant.h"
#include "headers/game_engine_type.h"
#include "headers/game_engine_universal_variant_flags.h"
#include "headers/game_engine_vehicles.h"
#include "headers/game_engine_weapons.h"

game_variant * build_game_variant_team_oddball(game_variant *result)
{
    /* keep bits 6+ (motion-tracker-no-enemies, game-mode), reset config bits 0-5, then set
     * allow_motion_tracker | allow_friendly_navpoints | generic_starting_equipment (0x23). */
    unsigned int flags = (result->universal_variant.flags & ~((1u << _motion_tracker_no_enemies_bit) - 1))
                       | (1u << _allow_motion_tracker_bit)
                       | (1u << _allow_friendly_navpoints_bit)
                       | (1u << _generic_starting_equipment_bit);

    result->game_engine_index = game_engine_oddball;
    result->universal_variant.health = 1.0f;
    result->universal_variant.lives = 0;
    result->universal_variant.goal_radar = 1;
    result->universal_variant.odd_man_out = 0;
    result->universal_variant.respawn_time = 300;
    result->universal_variant.respawn_time_growth = 0;
    result->universal_variant.score_to_win = 2;
    result->universal_variant.suicide_penalty = 150;
    result->universal_variant.teams = 1;
    result->universal_variant.weapon_set = _game_engine_weapons_classic;
    result->universal_variant.vehicles_respawn = 0;
    result->universal_variant.friendly_fire = 1;
    result->universal_variant.friendly_fire_penalty = 0;
    result->universal_variant.team_autobalance = 0;
    result->universal_variant.time_limit = 0;
    /* force game mode = classic: clear both game-mode bits (7,8 = 0x180), then set classic (bit 7). */
    result->universal_variant.flags =
        (flags & ~((1u << _game_mode_classic_bit) | (1u << _game_mode_standard_bit)))
        | (1u << _game_mode_classic_bit);
    /* vehicle set: preset field (bits 28-31) = none, all per-type counts 0 (was 0x10000000). */
    result->universal_variant.vehicle_set.value = 0;
    result->universal_variant.vehicle_set.__s1.preset = _game_engine_vehicles_none;
    result->universal_variant.vehicle_set2.value = 0;
    result->universal_variant.vehicle_set2.__s1.preset = _game_engine_vehicles_none;

    result->game_engine_variant.oddball.ball_spawn_count = 1;
    result->game_engine_variant.ctf.reset_on_capture = 0;
    result->game_engine_variant.ctf.assault = 0;
    result->game_engine_variant.oddball.oddball_ball_type = _oddball_normal;
    result->game_engine_variant.oddball.trait_with_ball = _game_trait_none;
    result->game_engine_variant.oddball.trait_without_ball = _game_trait_none;
    result->game_engine_variant.ctf.single_flag_time = 0;

    result->flags = 1;
    return result;
}
