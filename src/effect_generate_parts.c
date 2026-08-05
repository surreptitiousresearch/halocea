/* effect_generate_parts @0x836E3330 — spawn the one-shot "parts" (objects/decals/damage etc.) for the
 * effect's current event. For each part definition whose disposition matches the effect's violence flag,
 * it walks the part's resolved marker locations, derives each marker's world position/forward/up (from the
 * stored matrix, an object node, or the first-person weapon node), optionally overrides the orientation to
 * gravity/forward, checks the environment filter, and emits the part scaled by the effect's drivers. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/effect_part_definition_flags.h"
#include "headers/effect_part_type_scale_flags.h"
#include "headers/effect_disposition.h"
#include "headers/effect_datum.h"
#include "headers/effect_node_designator.h"
#include "headers/effect_definition.h"
#include "headers/effect_event_definition.h"
#include "headers/effect_part_definition.h"
#include "headers/effect_location_datum.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/effect_flags.h"
#include "headers/blam_data_globals.h"


extern effect_location_datum *effect_location_get_next_instance(const effect_datum *effect, int *location_datum_index, int16_t camera_mode);
extern real_matrix4x3 *first_person_weapon_get_node_matrix(int16_t local_player_index, int16_t node_index);
extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern uint8_t effect_allowed_by_environment(int16_t environment, const location *location, const real_point3d *world_point);
extern void effect_generate_part(const effect_datum *effect, const effect_part_definition *part_definition, effect_location_datum *location_instance, const real_point3d *world_point, const real_vector3d *world_forward, const real_vector3d *world_up, float scale);

extern float effect_scale(const effect_datum *effect, float value, unsigned int scale_a_flags, unsigned int scale_b_flags, char bit_index);
void effect_generate_parts(effect_datum *effect)
{
    effect_definition *definition = TAG_GET(effect_definition, effect->definition_index);
    effect_event_definition *event = (effect_event_definition *)definition->events.address + effect->event_index;
    const effect_part_definition *part_block = (const effect_part_definition *)event->parts.address;
    int part;

    if ( event->parts.count <= 0 )
        return;

    for ( part = 0; part < event->parts.count; part = (int16_t)(part + 1) )
    {
        /* recovered: (effect_part_definition *)(104 * part + block) -> &part_block[part] */
        const effect_part_definition *part_definition = &part_block[part];
        int location_index = part_definition->location_index;
        int location_cursor;
        effect_location_datum *instance;

        if ( location_index < 0 || location_index >= definition->locations.count || part_definition->reference.index == -1 )
            continue;

        /* disposition must match the effect's violence flag (nonviolent bit): violent vs nonviolent */
        if ( (((effect->flags >> _effect_nonviolent_bit) & 1) != 0
                  ? part_definition->disposition - _effect_disposition_violent
                  : part_definition->disposition - _effect_disposition_nonviolent) == 0 )
            continue;

        location_cursor = effect->location_datum_indices[location_index];
        instance = effect_location_get_next_instance(effect, &location_cursor, 0);
        while ( instance )
        {
            uint16_t node_designator = (uint16_t)instance->node_designator;
            real_point3d world_point;
            real_vector3d world_forward;
            real_vector3d world_up;

            if ( node_designator == 0xFFFF )
            {
                world_point.n[0] = instance->matrix.n[3][0];
                world_point.n[1] = instance->matrix.n[3][1];
                world_point.n[2] = instance->matrix.n[3][2];
                world_forward.n[0] = instance->matrix.n[0][0];
                world_forward.n[1] = instance->matrix.n[0][1];
                world_forward.n[2] = instance->matrix.n[0][2];
                world_up.n[0] = instance->matrix.n[2][0];
                world_up.n[1] = instance->matrix.n[2][1];
                world_up.n[2] = instance->matrix.n[2][2];
            }
            else
            {
                const real_matrix4x3 *node_matrix;
                if ( EFFECT_NODE_IS_FIRST_PERSON_WEAPON(node_designator) )
                    node_matrix = first_person_weapon_get_node_matrix(effect->local_player_index, EFFECT_NODE_DESIGNATOR_TO_INDEX(node_designator));
                else
                    node_matrix = object_get_node_matrix(effect->object_index,
                                      ((int16_t)node_designator == -1) ? -1 : EFFECT_NODE_DESIGNATOR_TO_INDEX(node_designator));
                matrix4x3_transform_point(node_matrix, (const real_point3d *)&instance->matrix.n[3], &world_point);
                matrix4x3_transform_normal(node_matrix, (const real_vector3d *)&instance->matrix.n[0], &world_forward);
                matrix4x3_transform_normal(node_matrix, (const real_vector3d *)&instance->matrix.n[2], &world_up);
            }

            if ( (part_definition->flags & (1u << _effect_part_definition_world_down_bit)) != 0 )
            {
                world_forward.n[0] = global_down3d->n[0];
                world_forward.n[1] = global_down3d->n[1];
                world_forward.n[2] = global_down3d->n[2];
                world_up.n[0] = global_forward3d->n[0];
                world_up.n[1] = global_forward3d->n[1];
                world_up.n[2] = global_forward3d->n[2];
            }

            if ( effect_allowed_by_environment(part_definition->environment, &effect->location, &world_point) )
            {
                /* DEVIATION: collapsed to effect_scale@0x836E1210 (donor is zero-xref, inlined at this site); value folds from literal 1.0f, bit_index folds from _effect_part_type_scale_bit (=5) */
                float scale = effect_scale(effect, 1.0f, part_definition->a_scales, part_definition->b_scales, _effect_part_type_scale_bit);
                effect_generate_part(effect, part_definition, instance, &world_point, &world_forward, &world_up, scale);
            }

            instance = effect_location_get_next_instance(effect, &location_cursor, 0);
        }
    }
}
