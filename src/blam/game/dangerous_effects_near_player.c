/* dangerous_effects_near_player @0x836E2210 — return 1 if any live effect with a non-zero danger radius
 * (and not flagged 0x8) has a location instance within (unit_radius + effect_radius) of any player's unit.
 * For each dangerous effect it walks every player's unit and every effect location instance, resolving the
 * instance's world position from its node designator: 0xFFFF is already world-space (the matrix
 * translation), the 0x8000 bit selects a first-person weapon node, otherwise an object node matrix is used
 * to transform the local marker position. The first instance found within range returns 1.
 *
 * DEVIATION: the decompiler's node-resolution path contained two `(__int16)node_designator == -1` tests
 * that are unreachable (the 0xFFFF/world case is handled first), so the dead `node = -1` branch is dropped. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/data_iterator.h"
#include "headers/effect_datum.h"
#include "headers/effect_node_designator.h"
#include "headers/effect_location_datum.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/global_tag_instances.h"
#include "headers/effect_definition.h"
#include "headers/unit_datum.h"
#include "headers/player_datum.h"
#include "headers/effect_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern void *datum_get(data_array *data, int index);
extern effect_location_datum *effect_location_get_next_instance(const effect_datum *effect, int *location_datum_index, int16_t camera_mode);
extern real_matrix4x3 *first_person_weapon_get_node_matrix(int16_t local_player_index, int16_t node_index);
extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);

uint8_t dangerous_effects_near_player(void)
{
    for (int index = data_next_index(effect_data, -1); index != -1; index = data_next_index(effect_data, index))
    {
        /* recovered: effect_data->data + 252*(u16)index -> datum_get (252 == effect datum_size incl. 2-byte salt header) */
        const effect_datum *effect = datum_get(effect_data, index);
        effect_definition *definition = TAG_GET(effect_definition, effect->definition_index);
        float effect_radius = definition->runtime_danger_radius;
        if ((effect->flags & (1u << _effect_stopped_bit)) != 0 || effect_radius == 0.0f)
            continue;

        data_iterator player_iter;
        data_iterator_new(&player_iter, player_data);
        for (player_datum *player = data_iterator_next(&player_iter); player;
             player = data_iterator_next(&player_iter))
        {
            if (player->unit_index == -1)
                continue;
            unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player->unit_index)->datum);

            int location_cursor[2];
            location_cursor[0] = effect->location_datum_indices[0];
            for (effect_location_datum *instance = effect_location_get_next_instance(effect, location_cursor, 0);
                 instance;
                 instance = effect_location_get_next_instance(effect, location_cursor, 0))
            {
                real_point3d world_position;
                unsigned int node_designator = (uint16_t)instance->node_designator;
                if (node_designator == 0xFFFF) /* world space: position is the matrix translation */
                {
                    world_position.n[0] = instance->matrix.n[3][0];
                    world_position.n[1] = instance->matrix.n[3][1];
                    world_position.n[2] = instance->matrix.n[3][2];
                }
                else if (EFFECT_NODE_IS_FIRST_PERSON_WEAPON(node_designator)) /* first-person weapon node */
                {
                    real_matrix4x3 *node_matrix =
                        first_person_weapon_get_node_matrix(effect->local_player_index, EFFECT_NODE_DESIGNATOR_TO_INDEX(node_designator));
                    matrix4x3_transform_point(node_matrix, (const real_point3d *)instance->matrix.n[3], &world_position);
                }
                else /* object node */
                {
                    real_matrix4x3 *node_matrix = object_get_node_matrix(effect->object_index, EFFECT_NODE_DESIGNATOR_TO_INDEX(node_designator));
                    matrix4x3_transform_point(node_matrix, (const real_point3d *)instance->matrix.n[3], &world_position);
                }

                float combined_radius = unit->object.bounding_sphere_radius + effect_radius;
                const float *unit_position = unit->object.bounding_sphere_center.n;
                float dx = world_position.n[0] - unit_position[0];
                float dy = world_position.n[1] - unit_position[1];
                float dz = world_position.n[2] - unit_position[2];
                float distance_sq = dy * dy + (dx * dx + dz * dz);
                if (distance_sq <= combined_radius * combined_radius)
                    return 1;
            }
        }
    }
    return 0;
}
