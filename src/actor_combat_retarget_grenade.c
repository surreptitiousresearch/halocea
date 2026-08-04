/* actor_combat_retarget_grenade @0x837B947C — re-validates a grenade throw against the actor VARIANT tag's
 * grenade_combat enemy/collateral-damage radii (floats abs +392/+412) at a new desired target point, and
 * if still acceptable, overwrites the actor's stored grenade target (dword triplet +1704).
 *
 * DEVIATION: the decompiler's `target_point`/`enemy_count_reference` locals are a stale-register artifact —
 * see actor_combat_check_collateral_damage.c's own deviation comment (its 4th/5th params are phantoms from
 * the float-ABI GPR skip). disasm_range(0x837B947C,0x837B94E0) confirms the real 4th argument passed to it
 * is simply this function's own `desired_grenade_target`, not anything derived from the tag instance; the
 * tag-instance computation is only ever used to load the two radius floats. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_variant_definition.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"


extern uint8_t actor_combat_check_collateral_damage(int actor_index, float enemy_radius, float collateral_damage_radius, const float *test_point, int16_t *threat_count_out);

unsigned __int8 actor_combat_retarget_grenade(int actor_index, const real_point3d *desired_grenade_target)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    /* disasm loads meta+0x58 = variant_definition_index (not definition_index): the enemy/collateral radii
     * are the actor VARIANT tag's grenade_combat floats (abs 0x188/0x19C), DB-typed fields. */
    actor_variant_definition *variant_definition =
        TAG_GET(actor_variant_definition, actor->meta.variant_definition_index);
    float enemy_radius = variant_definition->grenade_combat.enemy_radius;
    float collateral_damage_radius = variant_definition->grenade_combat.collateral_damage_radius;

    if ( !actor_combat_check_collateral_damage(actor_index, enemy_radius, collateral_damage_radius,
            (const float *)desired_grenade_target, nullptr) )
        return 0;

    actor->control.grenade_current_target = *desired_grenade_target;
    return 1;
}
