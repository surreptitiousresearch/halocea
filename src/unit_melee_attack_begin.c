/* unit_melee_attack_begin @0x836CED40 — starts a unit's melee (or continuous melee) attack. Fails if the
 * unit's animation is busy. It selects the melee animation state (continuous -> 32; otherwise the "overheat"
 * state 41 when already in state 40, else 30/31 depending on a biped flag), applies it, optionally forces
 * the throwing state, aligns facing toward the target, and arms the melee-impact counters. Returns 1 if the
 * attack began. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_animation.h"
#include "headers/real_vector2d.h"
#include "headers/biped_datum.h"
#include "headers/unit_definition.h"
#include "headers/biped_datum_flags.h"
#include "headers/unit_definition_flags.h"
#include "headers/unit_melee_attack_state.h"
#include "headers/unit_animation_state.h"
#include "headers/blam_data_globals.h"


extern uint8_t unit_animation_busy(unit_animation *animation);
extern uint8_t unit_animation_set_state(int unit_index, int16_t new_state);
extern void unit_align_facing(int unit_index, const real_vector2d *alignment_vector);

uint8_t unit_melee_attack_begin(int unit_index, uint8_t continuous, const real_vector2d *alignment_vector)
{
    biped_datum *unit = ((biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    unit_definition *definition = TAG_GET(unit_definition, unit->definition_index);
    unit_animation *animation = &unit->unit.animation;

    if (unit_animation_busy(animation))
        return 0;

    /* biped-specific melee flag: only consulted for object type 0 (biped) */
    int melee_flag = 0;
    if (!unit->object.type)
        melee_flag = unit->biped.flags & (1u << _biped_airborne_bit);   /* biped block only valid for type 0 */

    int16_t state;
    if (continuous)
        state = _unit_state_melee_continuous;
    else if ((uint8_t)animation->state == _unit_state_leap_airborne)
        state = _unit_state_leap_melee;
    else
        state = (melee_flag != 0) + _unit_state_melee_attack;   /* 30 melee / 31 melee-airborne */

    if (!unit_animation_set_state(unit_index, state) && !continuous)
        return 0;

    if ((definition->unit.flags & (1u << _unit_definition_melee_attack_is_fatal_bit)) != 0)
        animation->state = _unit_state_dying;

    if (alignment_vector)
        unit_align_facing(unit_index, alignment_vector);

    if (continuous)
    {
        unit->unit.melee_continuous_damage_effect_timer = 0;
        unit->unit.melee_attack_state = _unit_melee_attack_continuous;
    }
    else
    {
        unit->unit.melee_attack_state = _unit_melee_attack_starting;
    }

    return 1;
}
