/* object_damage_body @0x836B2FC0 — apply the body (health) portion of a damage event after the shield has
 * been accounted for. Scales the incoming damage by the material body multiplier, friendly-fire
 * resistance and difficulty, drains body vitality, accumulates per-region damage and destroys regions
 * past their threshold, handles deathless-cheat clamping, and — when the body crosses its destroyed
 * thresholds — destroys/depletes the object and spawns the body-damaged / area / localized damage effects.
 * Outputs the raw body damage and material multiplier through body_damage_reference /
 * body_damage_multiplier_reference, and ORs result bits into damage_flags_accumulator.
 *
 * Deviation: PPC float-skip ABI + many stack args. Verified the 13-arg signature against the prologue
 * (0x836B2FC0) and call site (0x836B4020): 8 GPR args, then stack args
 * (damage_flags_accumulator, body_damage_reference, body_damage_multiplier_reference,
 * total_damage in f1, should_do_actual_damage). effect_new_from_object color/impulse args are NULL float-skip phantoms.
 * material modifier indexes by damage_material->type. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/real_vector3d.h"
#include "headers/damage_resistance.h"
#include "headers/damage_resistance_flags.h"
#include "headers/damage_material.h"
#include "headers/damage_material_flags.h"
#include "headers/damage_definition.h"
#include "headers/damage_definition_flags.h"
#include "headers/damage_data.h"
#include "headers/damage_data_flags.h"
#include "headers/cheat_globals.h"
#include "headers/object_header_datum.h"
#include "headers/object_being_damaged_flags.h"
#include "headers/unit_datum.h"
#include "headers/damage_region.h"
#include "headers/object_region_flags.h"
#include "headers/object_type.h"
#include "headers/game_difficulty.h"
#include "headers/damage_category.h"
#include "headers/object_damage_flags.h"
#include "headers/game_team.h"
#include "headers/blam_data_globals.h"

#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
#include "headers/real_point3d.h"
extern uint8_t game_engine_running(void);
extern float object_get_maximum_body_vitality(int object_index, uint8_t ignore_difficulty);
extern float game_difficulty_get_value(int16_t value_type);
extern void object_destroy_region(int object_index, int16_t region_index);
extern void object_deplete_body(int object_index);
extern void object_destroy(int object_index);
extern int effect_new_from_object(int definition_index, int owner_object_index, int object_index, int16_t force_local_player_index, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);
extern void damage_effect_new_at_location(int effect_definition_index, int object_index, int16_t node_index, const real_point3d *position, const real_vector3d *direction, const real_vector3d *normal);

void object_damage_body(int object_index, int16_t region_index, int16_t node_index,
                        const real_vector3d *object_normal, const damage_resistance *damage_resistance,
                        const damage_material *damage_material, const damage_definition *damage_definition,
                        const damage_data *damage_data,
                        unsigned int *damage_flags_accumulator, float *body_damage_reference,
                        float *body_damage_multiplier_reference, float total_damage,
                        uint8_t should_do_actual_damage)
{
    /* DEVIATION: total_damage moved after the three pointer out-params — the binary's stack
     * slots 9/10/11 are the pointers (0x836B30B0 lwz r23,arg_54; 0x836B357C lwz arg_5C;
     * 0x836B3580 lwz arg_64) with total_damage in f1 (home slot arg_6C never read); placing
     * the float at position 9 would shift all three pointer slots. */
    float body_damage = damage_material->body_damage_multiplier * total_damage;
    uint8_t harmless_to_vehicle = 0;
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    /* parent vehicle with no driver and "ignores damage" resistance flag → no body damage */
    if ( (damage_resistance->flags & (1u << _damage_resistance_only_hurt_while_occupied_bit)) != 0 && object->object.type == object_type_vehicle
      && ((unit_datum *)object)->unit.driver_object_index == -1 )
        body_damage = 0.0f;

    if ( !game_engine_running() && damage_definition->category == _damage_category_falling
      && object->object.owner_team_index == _game_team_player )
        harmless_to_vehicle = 1;

    float maximum_body_vitality = object_get_maximum_body_vitality(object_index, harmless_to_vehicle);
    float inverse_max = maximum_body_vitality <= 0.0f ? 0.0f : (1.0f / maximum_body_vitality);

    float resisted = body_damage;
    if ( (*damage_flags_accumulator & (1u << _object_being_damaged_by_friendly_bit)) != 0 )  /* friendly fire */
    {
        resisted = (1.0f - damage_resistance->friendly_damage_resistance) * body_damage;
        if ( (*damage_flags_accumulator & (1u << _object_being_damaged_multiplied_by_difficulty_bit)) != 0 )
        {
            float difficulty = game_difficulty_get_value(0);
            /* fix: prior sweep corrupted the 0.0f literal into 'game_difficulty_level_easy.0f' */
            if ( difficulty > 0.0f )
                resisted = resisted / difficulty;
        }
    }

    float scaled_damage = damage_definition->material_modifiers[damage_material->type] * (resisted * inverse_max);

    if ( (object->object.damage_flags & (1u << _object_cannot_take_damage_bit)) == 0 )  /* not invulnerable body */
    {
        if ( body_damage > 0.0f && (damage_material->flags & (1u << _damage_material_head_bit)) != 0 )
        {
            unsigned int flags = damage_definition->flags;
            if ( (flags & (1u << _damage_can_cause_headshots_bit)) != 0 )  /* headshot: instant kill */
            {
                if ( game_engine_running()
                  || object->object.type != object_type_biped
                  || ((unit_datum *)object)->unit.player_index == -1 )
                {
                    if ( should_do_actual_damage == 1 )
                        object->object.body_vitality = 0.0f;
                    *damage_flags_accumulator |= (1u << _object_being_damaged_killed_instantly_bit);
                    if ( game_engine_running() )
                        *damage_flags_accumulator |= (1u << _object_being_damaged_force_hard_ping_bit);
                }
            }
            else if ( (flags & (1u << _damage_can_cause_multiplayer_headshots_bit)) != 0 )  /* double damage vs body in MP */
            {
                if ( game_engine_running() )
                {
                    scaled_damage = scaled_damage * 2.0f;
                    if ( scaled_damage > object->object.body_vitality )
                        *damage_flags_accumulator |= (1u << _object_being_damaged_force_hard_ping_bit);
                }
            }
        }
        if ( should_do_actual_damage == 1 )
            object->object.body_vitality = object->object.body_vitality - scaled_damage;
    }

    if ( should_do_actual_damage == 1 && region_index != -1
      && (object->object.regions_destroyed_flags & (1 << region_index)) == 0 )
    {
        unsigned char prior = object->object.region_damage[region_index];
        const damage_region *region =
            &((const damage_region *)damage_resistance->regions.address)[region_index];
        unsigned char accumulated = (int64_t)((scaled_damage * 255.0f) + prior);
        object->object.region_damage[region_index] = accumulated;
        float region_threshold = region->damage_threshold;
        if ( region_threshold > 0.0f && region_threshold < (accumulated * 0.0039215689f) )
        {
            object_destroy_region(object_index, region_index);
            *damage_flags_accumulator |= (1u << _object_being_damaged_region_destroyed_bit);
        }
    }

    float total_body = object->object.current_body_damage + scaled_damage;
    float total_body2 = object->object.recent_body_damage + scaled_damage;
    object->object.current_body_damage = total_body;
    object->object.recent_body_damage = total_body2;
    object->object.body_damage_decay_timer = 0;
    if ( total_body > 1.0f )
        object->object.current_body_damage = 1.0f;
    if ( total_body2 > 1.0f )
        object->object.recent_body_damage = 1.0f;

    if ( cheat.deathless_player && object->object.body_vitality < 0.0f )
    {
        int16_t object_type = object->object.type;
        if ( ((1 << object_type) & object_mask_unit) != 0 )  /* biped or vehicle */
        {
            int controlling_player = ((unit_datum *)object)->unit.player_index;
            char protected_unit = (controlling_player != -1);  /* unit is player-controlled */
            if ( !protected_unit && object_type == object_type_vehicle )
            {
                /* a vehicle with a deathless player seated must not be destroyed */
                for ( int seat = object->object.first_child_object_index; seat != -1; )
                {
                    unit_datum *occupant = (unit_datum *)
                        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, seat)->datum;
                    if ( ((1 << occupant->object.type) & object_mask_unit) != 0
                      && occupant->unit.player_index != -1 )
                    {
                        protected_unit = 1;
                        break;
                    }
                    seat = occupant->object.next_object_index;
                }
            }
            if ( protected_unit )
                object->object.body_vitality = 0.0f;
        }
    }

    if ( should_do_actual_damage == 1 )
    {
        float body_vitality = object->object.body_vitality;
        uint8_t harmless = 0;
        float maximum = object_get_maximum_body_vitality(object_index, harmless);
        float body_destroyed_threshold = damage_resistance->body_destroyed_threshold;
        float scaled_vitality = maximum * body_vitality;

        if ( body_destroyed_threshold >= 0.0f || scaled_vitality >= body_destroyed_threshold )
        {
            if ( scaled_vitality >= 0.0f )
            {
                if ( scaled_vitality < damage_resistance->body_damaged_effect_threshold
                  && (object->object.damage_flags & (1u << _object_passed_body_damage_threshold_bit)) == 0 )
                {
                    effect_new_from_object(damage_resistance->body_damaged_effect.index,
                                           object_index, object_index, -1, 0.0, 0.0, 0, 0);
                    object->object.damage_flags |= (1u << _object_passed_body_damage_threshold_bit);
                }
            }
            else if ( (object->object.damage_flags & (1u << _object_dead_bit)) == 0 )
            {
                for ( int16_t region = 0; region < damage_resistance->regions.count; ++region )
                {
                    if ( (((const damage_region *)damage_resistance->regions.address)[region].flags & (1u << _object_region_dies_when_object_dies_bit)) != 0 )
                        object_destroy_region(object_index, region);
                }
                object_deplete_body(object_index);
                *damage_flags_accumulator |= (1u << _object_being_damaged_body_depleted_bit);
            }
        }
        else
        {
            object_destroy(object_index);
            *damage_flags_accumulator |= (1u << _object_being_damaged_body_depleted_bit)
                                       | (1u << _object_being_damaged_body_destroyed_bit);
        }

        if ( (damage_data->flags & (1u << _damage_create_localized_effect_bit)) != 0 )
        {
            int localized = damage_resistance->localized_damage_effect.index;
            if ( localized != -1 )
                damage_effect_new_at_location(localized, object_index, node_index,
                                              &damage_data->epicenter, &damage_data->direction, object_normal);
        }

        if ( (damage_data->flags & (1u << _damage_area_of_effect_bit)) != 0 && body_damage > damage_resistance->area_damage_effect_threshold )
        {
            int area_effect = damage_resistance->area_damage_effect.index;
            if ( area_effect != -1 && damage_definition->category != _damage_category_flame )
                effect_new_from_object(area_effect, object_index, object_index, -1, 0.0, 0.0, 0, 0);
        }
    }

    *body_damage_reference = body_damage;
    *body_damage_multiplier_reference = damage_material->body_damage_multiplier;
}
