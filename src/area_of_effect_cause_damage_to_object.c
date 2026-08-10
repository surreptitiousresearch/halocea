/* area_of_effect_cause_damage_to_object @0x836B4680 — apply a radius/area damage event to a single
 * object. Performs line-of-sight collision tests from the damage epicenter to the object (either a
 * single direct ray, or a 4-ray spread sampled around a perpendicular basis when the damage effect has
 * a non-zero area_of_effect_core_radius and the object type permits it), applies friendly-fire
 * and team-suppression rules, scales the damage by the falloff between inner/outer radius, calls
 * object_cause_damage, and recurses to the object's child and to the next object in the area list.
 *
 * Damage-effect tag typed as damage_effect_definition (falloff/cutoff radii at its head). */

#include <stdint.h>
#include "headers/damage_data.h"
#include "headers/damage_effect_definition_flags.h"
#include "headers/damage_resistance_flags.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_result.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/_unit_definition.h"
#include "headers/unit_definition.h"
#include "headers/damage_effect_definition.h"
#include "headers/collision_model.h"
#include "headers/object_flags.h"
#include "headers/damage_definition_flags.h"
#include "headers/damage_data_flags.h"
#include "headers/object_type.h"
#include "headers/game_difficulty_value.h"
#include "headers/unit_definition_flags.h"
#include "headers/collision_test_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/ppc_intrinsics.h"

/* 0xC221: front-facing | structure | object vehicles/scenery/machines (bit indices per collision_test_flags.h;
 * no DB composite enum names this exact mask) */
#define AOE_DAMAGE_COLLISION_FLAGS \
    ((1u << _collision_test_front_facing_surfaces_bit) | (1u << _collision_test_structure_bit) | \
     (1u << _collision_test_objects_vehicles_bit) | (1u << _collision_test_objects_scenery_bit) | \
     (1u << _collision_test_objects_machines_bit))


extern int object_get_ultimate_parent(int object_index);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);
extern real_vector3d *perpendicular3d(const real_vector3d *a, real_vector3d *result);
extern float normalize3d(real_vector3d *v);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern float game_difficulty_get_value(int16_t value_type);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern void object_cause_damage(damage_data *damage_data, int object_index, int16_t node_index, int16_t region_index, int16_t material_index, const real_vector3d *object_normal);


void area_of_effect_cause_damage_to_object(damage_data *damage_data, int object_index,
                                           uint8_t damage_next_object)
{
    char did_damage = 0;
    char suppressed_by_difficulty = 0;

    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    damage_effect_definition *damage_effect =
        TAG_GET(damage_effect_definition, damage_data->definition_index);

    int can_damage = (object->object.flags & (1u << _object_invisible_bit)) == 0;
    _object_definition *object_definition =
        TAG_GET(_object_definition, object->definition_index);

    float *object_position = object->object.bounding_sphere_center.n;
    real_point3d *p_epicenter;

    real_vector3d to_object;
    real_vector3d basis_u, basis_v;       /* perpendicular basis for the 4-ray spread */
    real_vector3d ray;                    /* per-iteration spread ray */
    real_vector3d offset_ray;             /* from sampled point back toward object */
    real_point3d  point;
    collision_result spread_result, direct_result;

    if ( (object->object.flags & (1u << _object_invisible_bit)) != 0
      || ((1 << object->object.type) & object_mask_unit) == 0
      || damage_effect->damage.area_of_effect_core_radius <= 0.000099999997f )
    {
        /* Single direct line-of-sight ray from epicenter to object center. */
        int ultimate_parent = object_get_ultimate_parent(object_index);
        p_epicenter = &damage_data->epicenter;
        offset_ray.n[0] = object_position[0] - damage_data->epicenter.n[0];
        offset_ray.n[1] = object_position[1] - damage_data->epicenter.n[1];
        offset_ray.n[2] = object_position[2] - damage_data->epicenter.n[2];
        /* 0xC221 decomposed per collision_test_flags.h bit indices (no DB composite names this mask) */
        can_damage = collision_test_vector(AOE_DAMAGE_COLLISION_FLAGS, &damage_data->epicenter, &offset_ray,
                                           ultimate_parent, &direct_result) != 0 ? 0 : can_damage;
    }
    else
    {
        /* 4-ray spread: build a perpendicular basis around the epicenter->object vector, then probe
         * four offset points at the effect's area radius; damage is blocked only if all rays hit. */
        p_epicenter = &damage_data->epicenter;
        to_object.n[0] = object_position[0] - damage_data->epicenter.n[0];
        to_object.n[1] = object_position[1] - damage_data->epicenter.n[1];
        to_object.n[2] = object_position[2] - damage_data->epicenter.n[2];

        normalize3d(perpendicular3d(&to_object, &basis_u));
        basis_v.n[1] = (to_object.n[2] * basis_u.n[0]) - (basis_u.n[2] * to_object.n[0]);
        basis_v.n[0] = (basis_u.n[2] * to_object.n[1]) - (basis_u.n[1] * to_object.n[2]);
        basis_v.n[2] = (basis_u.n[1] * to_object.n[0]) - (to_object.n[1] * basis_u.n[0]);
        normalize3d(&basis_v);

        int all_blocked = 1;
        float radius = damage_effect->damage.area_of_effect_core_radius;

        for ( int i = 0; i < 4; ++i )
        {
            switch ( i )
            {
                case 0:
                    ray.n[0] = basis_u.n[0] * radius;
                    ray.n[1] = basis_u.n[1] * radius;
                    ray.n[2] = basis_u.n[2] * radius;
                    break;
                case 1:
                    ray.n[0] = basis_u.n[0] * -radius;
                    ray.n[1] = basis_u.n[1] * -radius;
                    ray.n[2] = basis_u.n[2] * -radius;
                    break;
                case 2:
                    ray.n[0] = basis_v.n[0] * radius;
                    ray.n[1] = basis_v.n[1] * radius;
                    ray.n[2] = basis_v.n[2] * radius;
                    break;
                default:
                    ray.n[0] = basis_v.n[0] * -radius;
                    ray.n[1] = basis_v.n[1] * -radius;
                    ray.n[2] = basis_v.n[2] * -radius;
                    break;
            }

            int parent_a = object_get_ultimate_parent(object_index);
            collision_test_vector(AOE_DAMAGE_COLLISION_FLAGS, &damage_data->epicenter, &ray, parent_a, &spread_result);
            point = spread_result.point;
            int parent_b = object_get_ultimate_parent(object_index);
            offset_ray.n[0] = object_position[0] - point.n[0];
            offset_ray.n[1] = object_position[1] - point.n[1];
            offset_ray.n[2] = object_position[2] - point.n[2];
            all_blocked = collision_test_vector(AOE_DAMAGE_COLLISION_FLAGS, &point, &offset_ray, parent_b,
                                                &spread_result) == 0 ? 0 : all_blocked;
        }

        if ( (char)all_blocked )
            can_damage = 0;
    }

    int flags = damage_effect->damage.flags;
    if ( (flags & (1u << _damage_does_not_hurt_owner_bit)) != 0 )
        can_damage = damage_data->owner_object_index == object_index ? 0 : can_damage;
    if ( (flags & (1u << _damage_does_not_hurt_friends_bit)) != 0 )
        can_damage = game_team_is_enemy(object->object.owner_team_index,
                                        damage_data->owner_team_index) == 0 ? 0 : can_damage;

    if ( (char)can_damage )
    {
        if ( (damage_effect->damage.flags & (1u << _damage_infection_form_pop_bit)) != 0 )
        {
            can_damage = 0;  /* default off; re-enabled only for valid biped/vehicle targets below */
            if ( ((1 << object->object.type) & object_mask_unit) != 0
              && (((unit_definition *)object_definition)->unit.flags & (1u << _unit_is_inconsequential)) != 0
              && object_index != damage_data->owner_object_index )
            {
                double instakill = game_difficulty_get_value(_game_difficulty_infection_form_toughness);
                can_damage = 1;
                if ( instakill > 0.0 || (damage_effect->damage.flags & (1u << _damage_does_not_hurt_infection_forms_bit)) != 0 )
                    can_damage = (damage_data->flags & (1u << _damage_damaged_one_object_bit)) == 0;
                if ( instakill > 0.0 )
                {
                    unsigned int *seed = get_global_random_seed_address();
                    if ( real_seed_random(seed) < (double)((float)instakill * 0.25f) )
                        can_damage = 0;
                }
                suppressed_by_difficulty = 1;
            }
        }
    }

    damage_data->flags |= 1u << _damage_area_of_effect_bit;

    if ( (char)can_damage )
    {
        float dx = object_position[0] - p_epicenter->n[0];
        float dy = object_position[1] - p_epicenter->n[1];
        float dz = object_position[2] - p_epicenter->n[2];
        damage_data->direction.n[0] = dx;
        damage_data->direction.n[1] = dy;
        damage_data->direction.n[2] = dz;

        float distance = __fsqrts((dz * dz) + ((dx * dx) + (dy * dy)));
        if ( __fabs(distance) < 0.000099999997f )
        {
            distance = 0.0f;
        }
        else
        {
            damage_data->direction.n[0] = (1.0f / distance) * dx;
            damage_data->direction.n[1] = (1.0f / distance) * dy;
            damage_data->direction.n[2] = (1.0f / distance) * dz;
        }

        float radius_scale = hcex_double_damage_radius ? 2.0f : 1.0f;
        float falloff;
        if ( ((damage_effect->cutoff_radius - damage_effect->falloff_radius) * radius_scale) <= 0.0f )
        {
            falloff = 1.0f;
        }
        else
        {
            falloff = 1.0f - (-((damage_effect->falloff_radius * radius_scale) - distance)
                              / ((damage_effect->cutoff_radius - damage_effect->falloff_radius) * radius_scale));
            if ( falloff < 0.0f )
                falloff = 0.0f;
            else if ( falloff > 1.0f )
                falloff = 1.0f;
        }

        if ( (damage_effect->flags & (1u << _damage_effect_dont_scale_damage_by_distance_bit)) == 0 )
            damage_data->scale = falloff;
        if ( falloff > 0.0f )
        {
            object_cause_damage(damage_data, object_index, -1, -1, -1, nullptr);
            did_damage = 1;
        }

        int collision_definition = object_definition->collision_model.index;
        if ( collision_definition != -1
          && (TAG_GET(collision_model, collision_definition)->resistance.flags & (1u << _damage_resistance_children_take_area_damage_bit)) != 0 )
        {
            int child_object = object->object.first_child_object_index;
            if ( child_object != -1 )
                area_of_effect_cause_damage_to_object(damage_data, child_object, 1u);
        }
    }

    if ( suppressed_by_difficulty && (!(char)can_damage || did_damage) )
        damage_data->flags |= 1u << _damage_damaged_one_object_bit; /* 0x40 */

    if ( damage_next_object )
    {
        int next_object = object->object.next_object_index;
        if ( next_object != -1 )
            area_of_effect_cause_damage_to_object(damage_data, next_object, 1u);
    }
}
