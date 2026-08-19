/* object_update @ 0x836F1178 — per-tick update of one object and (recursively) its attached children.
 * Skips entirely if the header's no-update flag (0x10) is set. Otherwise: counts garbage objects,
 * advances the object's animation interpolation frame cursor, runs the type-specific update, damage
 * update (only when the definition has a collision model), exports/computes function values and
 * change colors, recomputes node matrices unless the "no node matrices" flag (0x800000) is set, and
 * (re)connects dynamic lights for objects flagged for dynamic-lighting recompute (0x2000) that are not invisible or whose tag
 * has no model. Then it recurses into the first child and, when this object itself has a parent, into
 * its next sibling. Finally postprocesses node matrices, and on a network server (game_connection() == _game_connection_network_server)
 * marks the object network-at-rest when both its linear and angular velocities are ~zero. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_header_flags.h"
#include "headers/_object_definition.h"
#include "headers/object_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector3d.h"
#include "headers/game_connection.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


extern int16_t game_connection(void);
extern int object_type_update(int object_index);
extern void object_damage_update(int object_index);
extern void object_type_export_function_values(int object_index);
extern void object_compute_node_matrices(int object_index);
extern void object_compute_function_values(int object_index);
extern void object_compute_change_colors(int object_index);
extern void object_connect_lights(int object_index, uint8_t disconnect, uint8_t reconnect);
extern void object_postprocess_node_matrices(int object_index);

extern double __fabs(double x);

int object_update(int object_index)
{
    object_header_datum *header =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    object_datum *object = header->datum;
    _object_definition *object_definition =
        TAG_GET(_object_definition, object->definition_index);

    if ( (header->flags & (1u << _object_header_do_not_update_bit)) == 0 )
    {
        if ( (object->object.flags & (1u << _object_garbage_bit)) != 0 )
            ++object_globals->active_garbage_object_count;

        if ( object->object.animation.interpolation_frame_count )
        {
            int16_t elapsed = object->object.animation.interpolation_frame_index + 1;
            object->object.animation.interpolation_frame_index = elapsed;
            if ( elapsed >= object->object.animation.interpolation_frame_count )
                object->object.animation.interpolation_frame_count = 0;
        }

        object_type_update(object_index);
        if ( object_definition->collision_model.index != -1 )
            object_damage_update(object_index);
        object_type_export_function_values(object_index);
        if ( (object->object.flags & (1u << _object_do_not_recompute_node_matrices_bit)) == 0 )
            object_compute_node_matrices(object_index);
        object_compute_function_values(object_index);
        object_compute_change_colors(object_index);

        int object_flags = object->object.flags;
        if ( (object_flags & (1u << _object_dynamic_lighting_recompute_bit)) != 0
            && ((object_flags & (1u << _object_invisible_bit)) == 0
             || TAG_GET(_object_definition, object->definition_index)->model.index == -1) )
        {
            object_connect_lights(object_index, 1u, 1u);
        }

        int first_child = object->object.first_child_object_index;
        if ( first_child != -1 )
            object_update(first_child);
        if ( object->object.parent_object_index != -1 )
        {
            int next_sibling = object->object.next_object_index;
            if ( next_sibling != -1 )
                object_update(next_sibling);
        }

        object_postprocess_node_matrices(object_index);

        if ( game_connection() == _game_connection_network_server )
        {
            if ( __fabs((object->object.translational_velocity.n[0] - global_zero_vector3d->n[0])) < 0.000099999997
              && __fabs((object->object.translational_velocity.n[1] - global_zero_vector3d->n[1])) < 0.000099999997
              && __fabs((object->object.translational_velocity.n[2] - global_zero_vector3d->n[2])) < 0.000099999997
              && __fabs((object->object.angular_velocity.n[0] - global_zero_vector3d->n[0])) < 0.000099999997
              && __fabs((object->object.angular_velocity.n[1] - global_zero_vector3d->n[1])) < 0.000099999997
              && __fabs((object->object.angular_velocity.n[2] - global_zero_vector3d->n[2])) < 0.000099999997 )
            {
                object->object.network_at_rest = 1;
                return 1;
            }
            object->object.network_at_rest = 0;
        }
    }

    return 1;
}
