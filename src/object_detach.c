/* object_detach @0x836F0C48 — detach a child object from its parent. Resolves the child's parent and
 * the parent node it was attached to, bakes the child's local transform up into world space through the
 * parent node matrix, inherits the parent's linear and angular velocity, clears the parent/parent-node
 * links, reconnects the now-free child to the map, and re-activates it (object_activate).
 *
 * DEVIATION: two blocks of the decompiler output are verbatim compiler-inlined copies of named
 * functions and are emitted as the calls the source had: the post-disconnect re-fetch of the parent
 * + node_matrices.offset walk is object_get_node_matrices @0x836EBE68, and the tail
 * header-flag/cannot-be-activated/parent==-1 test is object_activate @0x836EBBE0 (both already
 * re-sourced; bodies match exactly). The decompiler's second parent pointer and re-derefed child
 * datum belonged to those inlinings. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

#include "headers/location.h"
extern void object_disconnect_from_map(int object_index);
extern real_matrix4x3 *object_get_node_matrices(int object_index);
extern void object_activate(int object_index);
extern void object_reconnect_to_map(int object_index, const location *location_in);
extern void matrix4x3_translation(real_matrix4x3 *matrix, const real_point3d *point);
extern void matrix4x3_rotation_from_vectors(real_matrix4x3 *matrix, const real_vector3d *forward, const real_vector3d *up);
extern void matrix4x3_multiply(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result);
extern void matrix4x3_to_point_and_vectors(const real_matrix4x3 *matrix, real_point3d *point, real_vector3d *forward, real_vector3d *up);

void object_detach(int child_object_index)
{
    object_header_datum *child_header =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child_object_index);
    object_datum *child = child_header->datum;

    object_datum *parent = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                                              child->object.parent_object_index)->datum;
    object_disconnect_from_map(child_object_index);

    const real_matrix4x3 *parent_node_matrix =
        object_get_node_matrices(child->object.parent_object_index) + child->object.parent_node_index;

    real_matrix4x3 local_translation;
    real_matrix4x3 local_rotation;
    real_matrix4x3 world_transform;
    matrix4x3_translation(&local_translation, &child->object.position);
    matrix4x3_rotation_from_vectors(&local_rotation, &child->object.forward, &child->object.up);
    matrix4x3_multiply(parent_node_matrix, &local_translation, &world_transform);
    matrix4x3_multiply(&world_transform, &local_rotation, &world_transform);
    matrix4x3_to_point_and_vectors(&world_transform, &child->object.position,
                                   &child->object.forward, &child->object.up);

    child->object.translational_velocity = parent->object.translational_velocity;
    child->object.angular_velocity = parent->object.angular_velocity;
    child->object.parent_node_index = -1;
    child->object.parent_object_index = -1;
    object_reconnect_to_map(child_object_index, 0);

    object_activate(child_object_index);
}
