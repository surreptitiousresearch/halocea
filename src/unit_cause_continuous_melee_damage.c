/* unit_cause_continuous_melee_damage @0x836CCB68 — per-tick damage for a sustained melee (e.g. the Flood's
 * continuous swipe). Only runs while the unit is in melee state 4 against a valid target object that has a
 * continuous-melee damage effect. When the per-tick cooldown (unit data +0x28A) has expired, it sweeps a short
 * vector through the target's collision model (origin -> origin offset by the unit's swing vector); on a hit it
 * records the impact point, transforms the struck plane into world space (flipping it for back-facing hits) and
 * re-arms the cooldown. The resulting damage_data is scaled to 1/30 (one tick) and applied to the target; the
 * cooldown is then decremented. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/damage_data.h"
#include "headers/damage_data_flags.h"
#include "headers/collision_model_instance.h"
#include "headers/collision_model_test_vector_result.h"
#include "headers/real_plane3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/unit_melee_attack_state.h"
#include "headers/collision_bsp_test_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"


#include "headers/real_matrix4x3.h"
extern uint8_t collision_model_instance_new(collision_model_instance *instance, int object_index);
extern uint8_t collision_model_test_vector(const collision_model_instance *instance, unsigned int flags, const real_point3d *point, const real_vector3d *vector, collision_model_test_vector_result *result);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern real_plane3d *matrix4x3_transform_plane(const real_matrix4x3 *matrix, const real_plane3d *plane, real_plane3d *result);
extern void damage_data_new(damage_data *damage_data, int definition_index);
extern void object_cause_damage(damage_data *damage_data, int object_index, int16_t node_index, int16_t region_index, int16_t material_index, const real_vector3d *object_normal);

void unit_cause_continuous_melee_damage(int unit_index)
{
    unit_datum *unit_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unit_definition *definition_tag = TAG_GET(unit_definition, unit_data->definition_index);

    if ( unit_data->unit.melee_attack_state != _unit_melee_attack_continuous )
        return;

    /* +284 is object.parent_object_index - continuous melee damages the object the unit is attached to */
    int target_object_index = unit_data->object.parent_object_index;
    if ( target_object_index == -1 || definition_tag->unit.melee_damage.index == -1 )
        return;

    char hit = 0;
    real_plane3d hit_plane;
    real_point3d impact;
    collision_model_test_vector_result test_result;

    if ( !unit_data->unit.melee_continuous_damage_effect_timer )
    {
        collision_model_instance instance;
        if ( collision_model_instance_new(&instance, target_object_index) )
        {
            real_point3d origin;
            object_get_origin(unit_index, &origin);

            real_vector3d sweep;
            sweep.n[0] = unit_data->object.forward.n[0] * 0.2f;
            sweep.n[1] = unit_data->object.forward.n[1] * 0.2f;
            sweep.n[2] = unit_data->object.forward.n[2] * 0.2f;

            origin.n[0] = origin.n[0] - (sweep.n[0] * 0.5f);
            origin.n[1] = origin.n[1] - (sweep.n[1] * 0.5f);
            origin.n[2] = origin.n[2] - (sweep.n[2] * 0.5f);

            if ( collision_model_test_vector(&instance, (1u << _collision_bsp_test_front_facing_surfaces_bit) | (1u << _collision_bsp_test_back_facing_surfaces_bit), &origin, &sweep, &test_result) )
            {
                impact.n[0] = (sweep.n[0] * test_result.bsp_result.t) + origin.n[0];
                impact.n[1] = (sweep.n[1] * test_result.bsp_result.t) + origin.n[1];
                impact.n[2] = (sweep.n[2] * test_result.bsp_result.t) + origin.n[2];

                matrix4x3_transform_plane(&instance.matrices[test_result.node_index],
                                          test_result.bsp_result.plane, &hit_plane);
                if ( test_result.bsp_result.plane_designator < 0 )
                {
                    hit_plane.n.n[0] = -hit_plane.n.n[0];
                    hit_plane.n.n[1] = -hit_plane.n.n[1];
                    hit_plane.n.n[2] = -hit_plane.n.n[2];
                    hit_plane.d = -hit_plane.d;
                }
                hit = 1;
            }
        }
    }

    damage_data damage;
    damage_data_new(&damage, definition_tag->unit.melee_damage.index);
    damage.owner_object_index = unit_index;
    damage.scale = SECONDS_PER_TICK;
    damage.owner_team_index = unit_data->object.owner_team_index;
    damage.owner_player_index = unit_data->unit.player_index;

    real_plane3d *plane;
    int16_t node_index, region_index, material_index;
    if ( hit )
    {
        unit_data->unit.melee_continuous_damage_effect_timer = 10;
        damage.flags |= (1u << _damage_create_localized_effect_bit);
        damage.direction.n[0] = unit_data->object.forward.n[0];
        damage.direction.n[1] = unit_data->object.forward.n[1];
        damage.direction.n[2] = unit_data->object.forward.n[2];
        damage.epicenter.n[0] = impact.n[0];
        damage.epicenter.n[1] = impact.n[1];
        damage.epicenter.n[2] = impact.n[2];
        damage.origin.n[0] = impact.n[0];
        damage.origin.n[1] = impact.n[1];
        damage.origin.n[2] = impact.n[2];
        node_index = test_result.node_index;
        region_index = test_result.region_index;
        material_index = test_result.bsp_result.material_index;
        plane = &hit_plane;
    }
    else
    {
        plane = 0;
        node_index = -1;
        region_index = -1;
        material_index = -1;
    }

    object_cause_damage(&damage, unit_data->object.parent_object_index, node_index, region_index, material_index,
                        plane ? &plane->n : 0);
    --unit_data->unit.melee_continuous_damage_effect_timer;
}
