/* actor_compute_prop_target_weight @0x837D6DC8 — score how desirable a prop is as a combat target. Returns 0
 * for props that can't/shouldn't be engaged (suppressed, non-enemy, not yet acknowledged, stale corpse, or
 * betrayal type). Otherwise combines a range-band term (range_band, from weapon/variant combat ranges), a
 * knowledge/threat term (threat_band), and continuity_bonus/acknowledged_bonus, scaled by 10 and biased by a
 * 1/(1+distance) falloff plus a current-target bonus.
 *
 * Definition fields resolved to the DB structs: weapon_definition weapon.ai_minimum_target_range @0x40C,
 * actor_variant_definition ranged_combat.combat_range_upper_bound @0xA0 and
 * ranged_combat.maximum_firing_range @0x74 (verified against disassembly).
 *
 * Deviation: the decompiler builds a fused 64-bit slot (high half threat_band, low half the integer sum) and
 * casts it to float; the high-half write is a dead artifact — the real conversion is of the sum. */

#include <stdint.h>
#include "headers/actor_definition.h"
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/weapon_definition.h"
#include "headers/actor_variant_definition.h"
#include "headers/actor_combat_status.h"
#include "headers/prop_perception_state.h"
#include "headers/actor_type.h"
#include "headers/ai_line_of_sight.h"
#include "headers/blam_data_globals.h"

extern uint8_t actor_has_ranged_weapon(uint16_t actor_index);
extern weapon_definition *actor_get_weapon_definition(int actor_index);
extern actor_variant_definition *actor_combat_get_firing_variant_definition(int actor_index);

float actor_compute_prop_target_weight(int actor_index, int prop_index)
{
    prop_datum  *prop  = DATA_ARRAY_ELEMENT(prop_data,  prop_datum,  prop_index);
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( prop->ignore
      || !prop->enemy
      || (unsigned int)prop->state < _prop_state_becoming_unacknowledged
      || (prop->dead && prop->dead_ticks >= 150)
      || prop->type == _actor_type_mounted_weapon )
    {
        return 0.0f;
    }

    int16_t continuity_bonus = 0;
    int16_t acknowledged_bonus = 0;
    float current_target_bias = 0.0f;
    actor_definition *character = TAG_GET(actor_definition, actor->meta.definition_index);
    const actor_variant_definition *variant = TAG_GET(const actor_variant_definition, actor->meta.variant_definition_index);

    int16_t range_band;
    if ( actor->meta.swarm )  /* swarm */
    {
        range_band = 0;
    }
    else if ( prop->unreachable_ticks > 0 )  /* unreachable */
    {
        range_band = 0;
    }
    else if ( !actor_has_ranged_weapon(actor_index) )
    {
        /* recovered: *(float *)(variant + 352/368) -> ranged_combat.melee_range/berserk_melee_range */
        float close_range = actor->emotions.berserk ? variant->ranged_combat.melee_range : variant->ranged_combat.berserk_melee_range;
        float distance = prop->distance;
        if ( distance < 2.0f && prop->state != _prop_state_inspected_orphan )
            range_band = 5;
        else if ( prop->vehicle_index != -1 )
            range_band = 0;
        else if ( prop->definitely_located && character->berserk.melee_leap_velocity == 0.0f )
            range_band = 0;
        else if ( prop->underwater == actor->input.underwater )
            range_band = (distance < close_range) ? 3 : 2;
        else
            range_band = 1;
    }
    else
    {
        weapon_definition *weapon = actor_get_weapon_definition(actor_index);
        actor_variant_definition *firing_variant = actor_combat_get_firing_variant_definition(actor_index);
        if ( weapon && prop->distance < (double)weapon->weapon.ai_minimum_target_range )
        {
            range_band = 2;
        }
        else if ( prop->underwater != actor->input.underwater )
        {
            range_band = 2;
        }
        else
        {
            float distance = prop->distance;
            if ( distance < 2.0f && prop->state != _prop_state_inspected_orphan )
                range_band = 5;
            /* recovered: *(float *)((char *)variant + 0xA0/0x74) -> ranged_combat.combat_range_upper_bound/maximum_firing_range */
            else if ( distance < firing_variant->ranged_combat.combat_range_upper_bound )
                range_band = 3;
            else
                range_band = (distance >= firing_variant->ranged_combat.maximum_firing_range) ? 1 : 2;
        }
    }

    int16_t threat_band;
    if ( prop->dead )  /* dead */
    {
        threat_band = 1;
    }
    else if ( actor->meta.swarm || !prop->currently_damaging_me || prop->unreachable_ticks )
    {
        int state = prop->state;
        if ( state < _prop_state_becoming_unacknowledged || state > _prop_state_acknowledged )
        {
            if ( prop->definitely_located )
                threat_band = 3;
            else
                threat_band = (state == _prop_state_uninspected_orphan) + 1;
        }
        else if ( !actor->meta.swarm && prop->unreachable_ticks > 0 )
        {
            threat_band = 3;
        }
        else if ( !actor->meta.swarm && prop->line_of_sight && prop->line_of_sight != _ai_line_of_sight_occluded )
        {
            threat_band = 3;
        }
        else if ( !actor->meta.swarm && prop->shooting && prop->quantized_facing <= 1 )
        {
            threat_band = 5;
        }
        else
        {
            threat_band = 4;
        }
    }
    else
    {
        threat_band = 6;
    }

    int target = actor->target.target_prop_index;
    if ( target == -1 )
    {
        if ( prop->player || prop_index == actor->meta.interesting_orphan_index )
            current_target_bias = 3.0f;
    }
    else if ( prop_index == target && actor->state.combat_status >= _actor_combat_status_definite )
    {
        continuity_bonus = 1;
    }
    if ( prop->preferred_target )
        acknowledged_bonus = 2;

    int total = acknowledged_bonus + continuity_bonus + range_band + threat_band;
    return (total * 10.0f)
         + ((5.0f / ((prop->distance * 0.1f) + 1.0f)) + current_target_bias);
}
