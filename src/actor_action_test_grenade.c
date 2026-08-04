/* actor_action_test_grenade @0x837F2EA0 — check whether an actor may plan a grenade throw right now. Fails
 * if the actor is in a vehicle (input.vehicle_index != -1). If part of an encounter, computes a per-tick
 * throw chance (game_difficulty_get_team_value(_game_difficulty_grenade_timer_scale, encounter->team_index) times the variant's grenade
 * chance @+0x1A8, doubled while "playfighting"), converts it to a tick delay, and blocks if the encounter's
 * last-grenade-thrown time (encounter+92, established in actor_action_try_to_throw_grenade.c) is set and
 * that delay hasn't elapsed yet. Otherwise delegates to actor_combat_find_grenade_target /
 * actor_combat_check_collateral_damage (using the variant's enemy_radius @+0x188, collateral_damage_radius
 * @+0x19C, and minimum_enemy_count @+0x186) and finally actor_combat_plan_grenade_trajectory (trajectory_type
 * @+0x182), returning whether a trajectory was successfully planned.
 *
 * DEVIATION: the raw decompile passed uninitialized locals (`v14`, `v13`) as
 * actor_combat_check_collateral_damage's target_point/enemy_count_reference arguments — disasm
 * (0x837F2FAC-0x837F3024) confirms these are actually `&grenade_target` and `&enemy_count`, the same
 * locals used immediately before (by actor_combat_find_grenade_target) and after (the minimum_enemy_count
 * comparison) — a plain decompiler register-binding miss, not a real bug in the binary. The `(__int16)`
 * truncation before adding the computed delay to the last-thrown time is reproduced as-is (grenade delays
 * never approach 32K ticks in practice, so this is harmless, not a signature of a real bug). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/encounter_datum.h"
#include "headers/actor_variant_definition.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"
#include "headers/game_difficulty_value.h"


extern int game_time_get(void);
extern float game_difficulty_get_team_value(int16_t value_type, int16_t team_index);
extern uint8_t actor_combat_find_grenade_target(uint16_t actor_index, real_point3d *grenade_target, int *grenade_target_prop_index, int *grenade_ignore_object_index);
/* def (src/actor_combat_check_collateral_damage.c) types the point arg `const float *`; matched here. */
extern uint8_t actor_combat_check_collateral_damage(int actor_index, float enemy_radius, float collateral_damage_radius, const float *test_point, int16_t *threat_count_out);
extern uint8_t actor_combat_plan_grenade_trajectory(int actor_index, int16_t trajectory_type, const real_point3d *grenade_target, int grenade_target_prop_index, int grenade_ignore_object_index);

uint8_t actor_action_test_grenade(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_variant_definition *variant_definition =
        TAG_GET(actor_variant_definition, actor->meta.variant_definition_index);
    int current_time = game_time_get();

    if ( actor->input.vehicle_index != -1 )
        return 0;

    unsigned __int8 blocked = 0;
    if ( actor->meta.encounter_index != -1 )
    {
        float grenade_chance = variant_definition->grenade_combat.encounter_grenade_timeout;
        encounter_datum *encounter =
            DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, actor->meta.encounter_index);

        float throw_chance = game_difficulty_get_team_value(_game_difficulty_grenade_timer_scale, encounter->team_index) * grenade_chance;
        if ( actor->external_orders.playfighting )
            throw_chance = throw_chance * 2.0f;

        /* recovered: *(int *)(encounter + 92) -> last_grenade_throw_time */
        int last_grenade_time = encounter->last_grenade_throw_time;
        blocked = 1;
        if ( last_grenade_time == -1 || (__int16)(int)(throw_chance * 30.0f) + last_grenade_time <= current_time )
            blocked = 0;
    }
    if ( blocked )
        return 0;

    real_point3d grenade_target;
    int grenade_target_prop_index;
    int grenade_ignore_object_index;
    if ( !actor_combat_find_grenade_target(actor_index, &grenade_target, &grenade_target_prop_index,
            &grenade_ignore_object_index) )
        return 0;

    __int16 enemy_count;
    if ( !actor_combat_check_collateral_damage(actor_index, variant_definition->grenade_combat.enemy_radius,
            variant_definition->grenade_combat.collateral_damage_radius, (const float *)&grenade_target, &enemy_count) )
        return 0;

    if ( enemy_count < variant_definition->grenade_combat.minimum_enemy_count )
        return 0;

    return actor_combat_plan_grenade_trajectory(actor_index, variant_definition->grenade_combat.trajectory_type,
            &grenade_target, grenade_target_prop_index, grenade_ignore_object_index) != 0;
}
