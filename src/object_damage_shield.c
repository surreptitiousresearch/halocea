/* object_damage_shield @0x836B2860 — apply a damage event to an object's shield. Computes how much of the
 * incoming damage the shield leaks through to the body (shield_damage_reference, reduced by the material
 * leak fraction and a shield-failure response curve), how much the shield itself absorbs
 * (being_damaged_flags), depletes/overpenetrates the shield when the hit exceeds remaining vitality, and
 * — when should_do_actual_damage is set — drains shield vitality, spawns the shield-damaged effect, and
 * accumulates the replicated shield-damage network record. should_do_damage_effects gates depletion.
 *
 * Deviation: PPC float-skip ABI + many stack args confused the decompiler. Verified signature against
 * the prologue (0x836B2860) and the call site in object_cause_damage (0x836B3F40): the 8th GPR arg is the
 * should_do_actual_damage byte, followed by two stack args (should_do_damage_effects, replicated record).
 * The effect_new_from_object color/impulse args are NULL (float-skip phantoms). v49's "effect_vector_field"
 * is really the byte offset of damage_definition->material_modifiers[shield_material_type]. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/damage_resistance.h"
#include "headers/damage_resistance_flags.h"
#include "headers/damage_material.h"
#include "headers/damage_definition.h"
#include "headers/damage_data.h"
#include "headers/damage_dealt_network_data.h"
#include "headers/object_header_datum.h"
#include "headers/object_being_damaged_flags.h"
#include "headers/game_difficulty.h"
#include "headers/damage_category.h"
#include "headers/damage_side_effect.h"
#include "headers/object_damage_flags.h"
#include "headers/game_team.h"
#include "headers/blam_data_globals.h"

#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
extern uint8_t game_engine_running(void);
extern float object_get_maximum_shield_vitality(int object_index, uint8_t ignore_difficulty);
extern float transition_function_evaluate(int16_t function_type, float value);
extern float game_difficulty_get_value(int16_t value_type);
extern int effect_new_from_object(int definition_index, int owner_object_index, int object_index, int16_t force_local_player_index, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);
extern void object_deplete_shield(int object_index);

void object_damage_shield(int object_index, const damage_resistance *damage_resistance,
                          const damage_material *damage_material, const damage_definition *damage_definition,
                          const damage_data *damage_data, float *being_damaged_flags,
                          float *shield_damage_reference, uint8_t should_do_actual_damage,
                          uint8_t should_do_damage_effects,
                          damage_dealt_network_data *replicated_damage_data)
{
    char negligible = 0;
    uint8_t harmless_to_vehicle = 0;

    float incoming = *shield_damage_reference;
    float shield_absorbed = incoming;

    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    replicated_damage_data->shield_damage = 0.0f;
    replicated_damage_data->shields_depleted = 0;

    if ( !game_engine_running() && damage_definition->category == _damage_category_falling )
        harmless_to_vehicle = (uint16_t)object->object.owner_team_index == _game_team_player;

    float *shield_vitality = &object->object.shield_vitality;
    float material_modifier = damage_definition->material_modifiers[damage_resistance->shield_material_type];

    if ( *shield_vitality <= 0.0f )
    {
        shield_absorbed = 0.0f;
        if ( should_do_actual_damage == 1 )
            *shield_vitality = 0.0f;
    }
    else
    {
        float maximum_shield_vitality = object_get_maximum_shield_vitality(object_index, harmless_to_vehicle);
        float inverse_max = maximum_shield_vitality <= 0.0f ? 0.0f : (1.0f / maximum_shield_vitality);

        /* damage_data->definition_index doubles as the object_being_damaged result-flags accumulator */
        if ( (damage_data->definition_index & (1u << _object_being_damaged_by_friendly_bit)) == 0 || (damage_resistance->flags & (1u << _damage_resistance_always_shields_friendly_damage_bit)) == 0 )
        {
            float failure_threshold = damage_resistance->shield_failure_threshold;
            shield_absorbed = (1.0f - damage_material->shield_leak_fraction) * incoming;
            if ( *shield_vitality <= failure_threshold && failure_threshold > 0.0f )
                shield_absorbed = ((1.0f - damage_resistance->maximum_shield_failure)
                        * transition_function_evaluate(damage_resistance->shield_failure_function,
                                                       *shield_vitality / damage_resistance->shield_failure_threshold)
                        + damage_resistance->maximum_shield_failure) * shield_absorbed;
        }

        if ( (object->object.damage_flags & (1u << _object_shield_over_charging_bit)) != 0 )
        {
            shield_absorbed = incoming;
            incoming = 0.0f;
        }
        else
        {
            if ( shield_absorbed < 0.0f )
                shield_absorbed = 0.0f;
            incoming = incoming - shield_absorbed;

            if ( (damage_data->definition_index & (1u << _object_being_damaged_by_friendly_bit)) != 0 && (damage_data->definition_index & (1u << _object_being_damaged_multiplied_by_difficulty_bit)) != 0 )
            {
                float difficulty = game_difficulty_get_value(0);
                /* fix: prior sweep corrupted the 0.0f literal into 'game_difficulty_level_easy.0f' */
                if ( difficulty > 0.0f )
                    shield_absorbed = shield_absorbed / difficulty;
            }

            float shield_damage = material_modifier * (damage_material->shield_damage_multiplier * shield_absorbed);
            if ( shield_damage < 0.000099999997f )
                negligible = 1;

            if ( (shield_damage * inverse_max) > (double)*shield_vitality
              || damage_definition->side_effect == _damage_side_effect_emp )
            {
                /* shield over-penetrated: pass the excess through to the body */
                float excess = (*shield_vitality * maximum_shield_vitality) - shield_damage;
                if ( -excess > 0.0f )
                    incoming = incoming - excess;
                if ( should_do_actual_damage == 1 )
                    *shield_vitality = 0.0f;
                if ( (object->object.damage_flags & (1u << _object_shield_depleted_bit)) == 0 && should_do_damage_effects == 1 )
                {
                    object_deplete_shield(object_index);
                    /* damage_data is const per DB proto but this field is the mutable result accumulator */
                    ((struct damage_data *)damage_data)->definition_index |= (1u << _object_being_damaged_shield_depleted_bit); /* struct tag: param name shadows the typedef */
                    replicated_damage_data->shields_depleted = 1;
                }
            }
            else
            {
                if ( should_do_actual_damage == 1 && (object->object.damage_flags & (1u << _object_cannot_take_damage_bit)) == 0 )
                    *shield_vitality = *shield_vitality - (shield_damage * inverse_max);
                if ( (object->object.damage_flags & (1u << _object_passed_shield_damage_threshold_bit)) == 0
                  && *shield_vitality < (double)damage_resistance->shield_damaged_effect_threshold )
                {
                    effect_new_from_object(damage_resistance->shield_damaged_effect.index,
                                           object_index, object_index, -1, 0.0, 0.0, 0, 0);
                    object->object.damage_flags |= (1u << _object_passed_shield_damage_threshold_bit);
                }
            }
        }

        if ( !negligible && should_do_actual_damage == 1 )
        {
            int16_t object_flags = object->object.damage_flags;
            float drained = (*shield_damage_reference - incoming) * inverse_max;
            object->object.shield_damage_decay_timer = 0;
            if ( (object_flags & (1u << _object_shield_depleted_bit)) == 0 )
                object->object.current_shield_damage = 1.0f;
            float recharge_fraction = object->object.current_shield_damage;
            float accumulated = object->object.recent_shield_damage + drained;
            object->object.recent_shield_damage = accumulated;
            if ( recharge_fraction > 1.0f )
                object->object.current_shield_damage = 1.0f;
            if ( accumulated > 1.0f )
                object->object.recent_shield_damage = 1.0f;
            replicated_damage_data->shield_damage = drained;
        }
    }

    if ( should_do_actual_damage == 1
      && (shield_absorbed >= damage_resistance->minimum_shield_stun_damage || *shield_vitality == 0.0f) )
        object->object.shield_stun_ticks = (int)(damage_resistance->shield_stun_time * 30.0f);

    *being_damaged_flags = shield_absorbed;
    *shield_damage_reference = incoming;
}
