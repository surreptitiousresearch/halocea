/* track_object_impulse_sound @ 0x837141CC — resolve the world-space position, orientation, velocity
 * and BSP location of a sound attached to an object node, filling out the sound_source. Returns 0 if
 * the object is gone or sits outside the BSP (no cluster); 1 on success. The attachment's local
 * position/forward are transformed by the object's node matrix (the body node when node_index == -1).
 *
 * Deviation: the decompiler rendered the game_location copy as a self-referential scratch struct
 * whose leaf_index was overwritten with &source->location (a register-reuse artifact of the inlined
 * struct copy). The actual operation is a plain copy of the object's location into the source. */

#include <stdint.h>
#include "headers/sound_source.h"
#include "headers/sound_attachment_data.h"
#include "headers/location.h"
#include "headers/real_matrix4x3.h"
#include "headers/object_type.h"

#include "headers/real_vector3d.h"
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern location *object_get_location(int object_index, location *location_out);
extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern void object_get_velocities(int object_index, real_vector3d *translational_velocity, real_vector3d *angular_velocity);

uint8_t track_object_impulse_sound(int object_index, const sound_attachment_data *attachment_data,
                                   sound_source *source)
{
    location object_location;
    const real_matrix4x3 *node_matrix;

    if ( !object_try_and_get_and_verify_type(object_index, object_mask_all) )
        return 0;

    object_get_location(object_index, &object_location);
    if ( (uint16_t)object_location.cluster_index == 0xFFFF )
        return 0;

    node_matrix = object_get_node_matrix(object_index,
                                         attachment_data->node_index == -1 ? 0 : attachment_data->node_index);

    source->location.game_location = object_location;
    matrix4x3_transform_point(node_matrix, &attachment_data->position, &source->location.position);
    matrix4x3_transform_normal(node_matrix, &attachment_data->forward, &source->location.forward);
    object_get_velocities(object_index, &source->location.translational_velocity, 0);
    return 1;
}
